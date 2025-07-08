#include "donut/session.hpp"

#include "donut/core.hpp"
#include "donut/input.hpp"
#include "donut/parameter.hpp"

#include <csignal>
#include <iostream>
#include <mutex>
#include <termios.h>
#include <thread>

// input handling
//   xXyYzZ: increase/decrease rotation speed in that direction
//   ctrl-c: quit
//   TODO: space: start/pause
//   TODO: p: toggle parallel/point
//   TODO: wasdqe: controls light
//      - par: rotate about each axis
//      - pnt: move along direction
//   TODO: hjkl: move camera (h and l effectively rotates the shape)
//   TODO: c: show config
//   TODO: f: next frame (only when paused)

// TODO: move compute_thread buffer index to (output + 3), i.e. only keep 3 cached frames

// TODO: implement config page
// show current rps, light src, light vec, grayscale, etc.

namespace donut::session {

const int DEBOUNCE_MS = 50;

std::atomic<bool> terminate(false);
std::atomic<int> advance(-1); // -1: keep going, >=0: play that many frames and stop
std::atomic<int> compute_idx(0);
std::atomic<int> output_idx(0);

std::mutex buffer_mtx;
int buffer_cnt = 0;
std::array<grd, MAX_BUFFER_SIZE> buffer;

std::condition_variable cv_compute;
std::condition_variable cv_output;

termios orig;

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
  cv_compute.notify_all();
  cv_output.notify_all();
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
      if (std::chrono::duration_cast<std::chrono::milliseconds>(now - last_input).count() < DEBOUNCE_MS) continue;
      last_input = now;

      input::handle_user_input(n, buf);
    }
  }
}

void _compute_thread(ves points, ves normals) {
  int wid = buffer[0].size();
  int hei = buffer[0][0].size();
  grd canvas(wid, std::vector<dbl>(hei, -1));
  vec rps;
  dbl fps;

  while (!terminate) {
    {
      std::unique_lock<std::mutex> lock(buffer_mtx);
      cv_compute.wait(lock, [] {
        return buffer_cnt < MAX_BUFFER_SIZE || terminate;
      });
      if (terminate) break;
    }

    core::draw(canvas, points, normals);
    {
      std::scoped_lock<std::mutex> lock(parameter::params_mtx);
      rps = parameter::cur_params.shape.rps;
      fps = parameter::cur_params.display.fps;
    }
    for (auto& r : rps) r /= fps;
    core::rotate_shape(points, normals, rps);
    
    int id = compute_idx;
    {
      std::scoped_lock<std::mutex> lock(buffer_mtx);
      buffer[id] = canvas;
      buffer_cnt += 1;
    }

    compute_idx = (compute_idx + 1) % MAX_BUFFER_SIZE;
    cv_output.notify_one();
  }
}

void _output_thread() {
  int wid = buffer[0].size();
  int hei = buffer[0][0].size();
  grd canvas(wid, std::vector<dbl>(hei));
  grd old_canvas(wid, std::vector<dbl>(hei, -1));
  int fps;

  while (!terminate) {
    if (advance == 0) continue;
    else if (advance > 0) --advance;

    {
      std::unique_lock<std::mutex> lock(buffer_mtx);
      cv_output.wait(lock, [] {
        return buffer_cnt > 0 || terminate;
      });
      if (terminate) break;
    }

    int id = output_idx;
    {
      std::scoped_lock<std::mutex> lock(buffer_mtx);
      canvas = buffer[id];
      buffer_cnt -= 1;
    }

    output_idx = (output_idx + 1) % MAX_BUFFER_SIZE;
    cv_compute.notify_one();

    core::update_screen(canvas, old_canvas);
    {
      std::scoped_lock<std::mutex> lock(parameter::params_mtx);
      fps = parameter::cur_params.display.fps;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds((int64_t) (1000 / fps)));
  }
}

}
