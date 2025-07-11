#include "donut/session.hpp"

#include "donut/core.hpp"
#include "donut/control.hpp"
#include "donut/parameter.hpp"
#include "donut/shapes.hpp"

#include <csignal>
#include <iostream>
#include <mutex>
#include <termios.h>
#include <thread>

namespace donut::session {

const int DEBOUNCE_MS = 50;

std::atomic<bool> terminate(false);
std::atomic<int> advance(-1); // -1: keep going, >=0: play that many frames and stop

std::mutex buffer_mtx;
std::array<grd, BUFFER_SIZE> buffer;
int buffer_cnt = 0;

std::mutex idx_mtx; // NOTE: only for compute_idx
uint64_t output_idx = 0;
uint64_t compute_idx = 0;

std::mutex hist_mtx;
std::array<ves, BUFFER_SIZE> points_hist;
std::array<ves, BUFFER_SIZE> normals_hist;
bool retrieve = false;

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

void entry() { // specify shape, params (specified in cli or config file)
  std::signal(SIGINT, sigint_handler);
  terminal_mode_set();
  
  donut::parameter::setup_char_ratio(donut::parameter::cur_params);
  donut::parameter::setup_camera_movement(donut::parameter::cur_params);
  donut::control::setup_default_keymap(donut::control::key_mappings);

  // setup buffer
  auto [wid, hei] = core::get_terminal_size();
  for (auto& canvas : buffer) {
    canvas.resize(wid);
    for (auto& row : canvas) {
      row.resize(hei);
    }
  }

  // get params (either default of read from file)
  // if (config files exists) {
  // }

  // obtain shape
  dbl r1 = 60;
  dbl r2 = 30;
  auto [points, normals] = shapes::donut(r1, r2);

  std::thread input_thread(_input_thread);
  std::thread output_thread(_output_thread);
  std::thread compute_thread(_compute_thread, points, normals);

  input_thread.join();
  output_thread.join();
  compute_thread.join();

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
      ssize_t chars = read(STDIN_FILENO, buf, sizeof(buf));

      auto now = std::chrono::steady_clock::now();
      if (std::chrono::duration_cast<std::chrono::milliseconds>(now - last_input).count() < DEBOUNCE_MS) continue;
      last_input = now;

      control::handle_user_input(chars, buf);
    }
  }
}

void _compute_thread(ves points, ves normals) {
  int wid = buffer[0].size();
  int hei = buffer[0][0].size();
  grd canvas(wid, std::vector<char>(hei, -1));
  vec rps;
  dbl fps;

  while (!terminate) {
    // wait for available space in buffer
    {
      std::unique_lock<std::mutex> lock(buffer_mtx);
      cv_compute.wait(lock, [] {
        return buffer_cnt < BUFFER_SIZE || terminate;
      });
      if (terminate) break;
    }

    // retrieve data
    {
      LOCK(idx_mtx, hist_mtx);
      if (retrieve) {
        uint64_t idx = compute_idx % BUFFER_SIZE;
        points = points_hist[idx];
        normals = normals_hist[idx];
        retrieve = false;
      }
    }

    // compute frame and place into buffer & store data into history
    core::draw(canvas, points, normals);
    {
      LOCK(idx_mtx, buffer_mtx);
      uint64_t idx = (compute_idx++) % BUFFER_SIZE;
      buffer[idx] = canvas;
      buffer_cnt += 1;
      points_hist[idx] = points;
      normals_hist[idx] = normals;
    }
    cv_output.notify_one();

    // update data points for next frame
    {
      using namespace parameter;
      LOCK(params_mtx);
      rps = parameter::cur_params.shape.rps;
      fps = parameter::cur_params.display.fps;
    }
    for (auto& r : rps) r /= fps;
    core::rotate_shape(points, normals, rps);
  }
}

void _output_thread() {
  int wid = buffer[0].size();
  int hei = buffer[0][0].size();
  grd canvas(wid, std::vector<char>(hei));
  grd old_canvas(wid, std::vector<char>(hei, -1));
  int fps;

  while (!terminate) {
    if (advance == 0) continue;
    else if (advance > 0) --advance;

    // wait for available frame in buffer
    {
      std::unique_lock<std::mutex> lock(buffer_mtx);
      cv_output.wait(lock, [] {
        return buffer_cnt > 0 || terminate;
      });
      if (terminate) break;
    }

    // copy frame from buffer and output to screen
    {
      LOCK(buffer_mtx);
      uint64_t idx = (output_idx++) % BUFFER_SIZE;
      canvas = buffer[idx];
      buffer_cnt -= 1;
    }
    cv_compute.notify_one();
    core::update_screen(canvas, old_canvas);

    // sleep for 1/fps
    {
      using namespace parameter;
      LOCK(params_mtx);
      fps = cur_params.display.fps;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds((int64_t) (1000 / fps)));
  }
}

}
