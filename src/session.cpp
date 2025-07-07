#include "donut/session.hpp"

#include "donut/core.hpp"
#include "donut/parameter.hpp"

#include <csignal>
#include <iostream>
#include <mutex>
#include <termios.h>
#include <thread>

namespace donut::session {

std::atomic<bool> terminate(false);

termios orig;

std::mutex buffer_mtx;

std::condition_variable cv_compute;
std::condition_variable cv_output;
int buffer_cnt = 0;
std::array<grd, MAX_BUFFER_SIZE> buffer;

void terminal_mode_set() {
  tcgetattr(STDIN_FILENO, &orig);
  termios raw = orig;
  raw.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

  std::cout << "\x1b[2J\x1b[H"; // clear screen
  std::cout << "\x1b[?25l";     // hide cursor
}

void terminal_mode_reset() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig);

  std::cout << "\x1b[2J\x1b[H"; // clear screen
  std::cout << "\x1b[?25h";     // show cursor
  std::cout << "\x1b[H";        // move cursor to home
}

void sigint_handler(int) {
  terminate = true;
  session::cv_compute.notify_all();
  session::cv_output.notify_all();
  terminal_mode_reset();
}

void entry() {
  std::signal(SIGINT, sigint_handler);
  terminal_mode_set();

  // setup buffer
  int hei, wid;
  std::tie(hei, wid) = core::get_terminal_size();
  for (auto& canvas : buffer) {
    canvas.resize(wid);
    for (auto& row : canvas) {
      row.resize(hei);
    }
  }

  std::thread input_thread(_input_thread);
  std::thread output_thread(_output_thread);
  // std::thread compute_thread(_compute_thread);

  input_thread.join();
  output_thread.join();
  // compute_thread.join();

  terminal_mode_reset();
}

void _input_thread() {
  constexpr int debounce_ms = 50;
  auto last_input = std::chrono::steady_clock::now();
    fd_set readfds;

  while (!terminate) {
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);
    timeval timeout = {0, 100000}; // 100 ms
    int ready = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout);

    if (ready > 0 && FD_ISSET(STDIN_FILENO, &readfds)) {
      char buf[3];
      ssize_t n = read(STDIN_FILENO, buf, sizeof(buf));

      auto now = std::chrono::steady_clock::now();
      if (std::chrono::duration_cast<std::chrono::milliseconds>(now - last_input).count() < debounce_ms) {
        continue;
      }
      last_input = now;

      if (n == 1 && buf[0] == 'q') {
        std::cout << "\n[DonutSession] Quitting input loop...\n";
        terminate = true;
      } else if (n == 3 && buf[0] == '\x1b' && buf[1] == '[') {
        switch (buf[2]) {
          case 'A': std::cout << "↑ Up\n"; break;
          case 'B': std::cout << "↓ Down\n"; break;
          case 'C': std::cout << "→ Right\n"; break;
          case 'D': std::cout << "← Left\n"; break;
        }
      }
    }
  }
}

void _compute_thread(ves points, ves normals) {
  int wid = buffer[0].size();
  int hei = buffer[0][0].size();
  grd canvas(wid, std::vector<dbl>(hei, -1));

  for (int id = 0; !session::terminate; id = (id + 1) % MAX_BUFFER_SIZE) {
    {
      std::unique_lock<std::mutex> lock(buffer_mtx);
      cv_compute.wait(lock, [] {
        return buffer_cnt < MAX_BUFFER_SIZE || session::terminate;
      });
      if (session::terminate) break;
    }

    core::draw(canvas, points, normals);
    core::rotate_shape(points, normals, parameter::params.shape.rps);

    {
      std::scoped_lock<std::mutex> lock(buffer_mtx);
      buffer[id] = canvas;
      buffer_cnt += 1;
    }

    cv_output.notify_one();
  }
}

void _output_thread() {
  int wid = buffer[0].size();
  int hei = buffer[0][0].size();
  grd canvas(wid, std::vector<dbl>(hei));
  grd old_canvas(wid, std::vector<dbl>(hei, -1));

  for (int id = 0; ; id = (id + 1) % MAX_BUFFER_SIZE) {
    {
      std::unique_lock<std::mutex> lock(buffer_mtx);
      cv_output.wait(lock, [] {
        return buffer_cnt > 0 || session::terminate;
      });
      if (session::terminate) break;
    }

    {
      std::scoped_lock<std::mutex> lock(buffer_mtx);
      canvas = buffer[id];
      buffer_cnt -= 1;
    }

    cv_compute.notify_one();

    core::update_screen(canvas, old_canvas);
    std::this_thread::sleep_for(std::chrono::milliseconds((int64_t) (1000 / parameter::params.display.fps)));
  }
}

}
