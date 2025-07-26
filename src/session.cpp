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

std::atomic<bool> terminate(false);
std::atomic<int> advance(-1); // -1: keep going, >=0: play that many frames and stop

std::mutex buffer_mtx;
std::vector<grd> buffer;
int buffer_cnt = 0;

std::mutex idx_mtx; // NOTE: only for compute_idx
uint64_t output_idx = 0;
uint64_t compute_idx = 0;

std::mutex hist_mtx;
std::vector<ves> points_hist;
std::vector<ves> normals_hist;
bool retrieve = false;

std::condition_variable cv_compute;
std::condition_variable cv_output;

bool is_interactive = false;

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
}

void entry() { // specify shape, params (specified in cli or config file)
  std::signal(SIGINT, sigint_handler);
  terminal_mode_set();
  
  parameter::try_setup_char_ratio(parameter::immutable_params.display);
  parameter::setup_camera_movement(parameter::mutable_params.camera);
  control::setup_default_keymap(control::keymap);

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
      if (chars != 1) continue;

      auto now = std::chrono::steady_clock::now();
      if (std::chrono::duration_cast<std::chrono::milliseconds>(now - last_input).count() < parameter::immutable_params.control.debounce) continue;
      last_input = now;

      control::handle_user_input(buf[0]);
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
        return buffer_cnt < parameter::immutable_params.animation.buffer_size || terminate;
      });
      if (terminate) break;
    }

    // retrieve data
    {
      LOCK(idx_mtx, hist_mtx);
      if (retrieve) {
        uint64_t idx = compute_idx % parameter::immutable_params.animation.buffer_size;
        points = points_hist[idx];
        normals = normals_hist[idx];
        retrieve = false;
      }
    }

    // compute frame and place into buffer & store data into history
    core::draw(canvas, points, normals);
    {
      LOCK(idx_mtx, buffer_mtx);
      uint64_t idx = (compute_idx++) % parameter::immutable_params.animation.buffer_size;
      buffer[idx] = canvas;
      buffer_cnt += 1;
      points_hist[idx] = points;
      normals_hist[idx] = normals;
    }
    cv_output.notify_one();

    // update data points for next frame
    {
      using namespace parameter;
      COND_LOCK(is_interactive, params_mtx);
      rps = mutable_params.shape.rps;
      fps = immutable_params.display.fps;
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
      uint64_t idx = (output_idx++) % parameter::immutable_params.animation.buffer_size;
      canvas = buffer[idx];
      buffer_cnt -= 1;
    }
    cv_compute.notify_one();
    core::update_screen(canvas, old_canvas);

    // sleep for 1/fps
    fps = parameter::immutable_params.display.fps;
    std::this_thread::sleep_for(std::chrono::milliseconds((int64_t) (1000 / fps)));
  }
}

}
