#include "donut/donut.hpp"

#include <iostream>
#include <thread>
#include <csignal>

void test_terminal_size() {
  std::pair<int, int> ts = donut::core::get_terminal_size();
  std::cout << ts.first << ' ' << ts.second << std::endl;
}

void test_animate() {
  dbl r1 = 60, r2 = 30;
  dbl w = 50, h = 80, l = 40;
  ves points, normals;
  tie(points, normals) = donut::shapes::donut(r1, r2);
  // tie(points, normals) = ellipsoid(w, h, l);
  // tie(points, normals) = donut::shapes::mister_donut(60, 24, 8);
  // tie(points, normals) = donut::shapes::methane(24, 12, 4, 85);
  
  std::signal(SIGINT, donut::session::sigint_handler);
  donut::session::terminal_mode_set();

  // setup buffer
  int hei, wid;
  std::tie(hei, wid) = donut::core::get_terminal_size();
  for (auto& canvas : donut::session::buffer) {
    canvas.resize(wid);
    for (auto& row : canvas) {
      row.resize(hei);
    }
  }

  std::thread input_thread(donut::session::_input_thread);
  std::thread output_thread(donut::session::_output_thread);
  std::thread compute_thread(donut::session::_compute_thread, points, normals);

  input_thread.join();
  output_thread.join();
  compute_thread.join();

  donut::session::terminal_mode_reset();
}

void test_termios() {
  dbl r1 = 60, r2 = 30;
  // auto [points, normals] = donut::shapes::mister_donut(r1, r2, 8);
  auto [points, normals] = donut::shapes::donut(r1, r2);
  
  std::signal(SIGINT, donut::session::sigint_handler);
  donut::session::terminal_mode_set();
  donut::parameter::setup_char_ratio();
  donut::parameter::setup_camera_movement();

  // setup buffer
  auto [hei, wid] = donut::core::get_terminal_size();
  for (auto& canvas : donut::session::buffer) {
    canvas.resize(wid);
    for (auto& row : canvas) {
      row.resize(hei);
    }
  }

  std::thread input_thread(donut::session::_input_thread);
  std::thread output_thread(donut::session::_output_thread);
  std::thread compute_thread(donut::session::_compute_thread, points, normals);

  input_thread.join();
  output_thread.join();
  compute_thread.join();

  donut::session::terminal_mode_reset();
}

int main() {
  // test_terminal_size();
  // test_draw();

  // donut::session::entry();
  // test_animate();

  test_termios();
}
