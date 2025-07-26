#include "donut/donut.hpp"

#include <iostream>
#include <thread>
#include <csignal>

#include <CLI/CLI.hpp>
#include <toml++/toml.hpp>

using namespace donut;

void test_terminal_size() {
  std::pair<int, int> ts = core::get_terminal_size();
  std::cout << ts.first << ' ' << ts.second << std::endl;
}

void test_animate() {
  dbl r1 = 60, r2 = 30;
  dbl w = 50, h = 80, l = 40;
  ves points, normals;
  tie(points, normals) = shapes::donut(r1, r2);
  // tie(points, normals) = ellipsoid(w, h, l);
  // tie(points, normals) = shapes::mister_donut(60, 24, 8);
  // tie(points, normals) = shapes::methane(24, 12, 4, 85);
  
  std::signal(SIGINT, session::sigint_handler);
  session::terminal_mode_set();

  // setup buffer
  int hei, wid;
  std::tie(hei, wid) = core::get_terminal_size();
  for (auto& canvas : session::buffer) {
    canvas.resize(wid);
    for (auto& row : canvas) {
      row.resize(hei);
    }
  }

  std::thread input_thread(session::_input_thread);
  std::thread output_thread(session::_output_thread);
  std::thread compute_thread(session::_compute_thread, points, normals);

  input_thread.join();
  output_thread.join();
  compute_thread.join();

  session::terminal_mode_reset();
}

void test_termios() {
  dbl r1 = 60, r2 = 30;
  // auto [points, normals] = shapes::mister_donut(r1, r2, 8);
  auto [points, normals] = shapes::donut(r1, r2);
  
  std::signal(SIGINT, session::sigint_handler);
  session::terminal_mode_set();
  parameter::try_setup_char_ratio(parameter::cur_params);
  parameter::setup_camera_movement(parameter::cur_params);

  // setup buffer
  auto [hei, wid] = core::get_terminal_size();
  for (auto& canvas : session::buffer) {
    canvas.resize(wid);
    for (auto& row : canvas) {
      row.resize(hei);
    }
  }

  std::thread input_thread(session::_input_thread);
  std::thread output_thread(session::_output_thread);
  std::thread compute_thread(session::_compute_thread, points, normals);

  input_thread.join();
  output_thread.join();
  compute_thread.join();

  session::terminal_mode_reset();
}

void test_toml() {
  std::string config_file = "/Users/leo/Desktop/code/projects/donut/tmp/config.toml";
  std::string write_to = "/Users/leo/Desktop/code/projects/donut/tmp/output.toml";

  dbl r1 = 60, r2 = 30;
  auto [points, normals] = shapes::donut(r1, r2);
  
  std::signal(SIGINT, session::sigint_handler);
  session::terminal_mode_set();
  parameter::try_setup_char_ratio(parameter::cur_params);
  parameter::setup_camera_movement(parameter::cur_params);

  vst errors = config::parse_config(parameter::cur_params, control::keymap, config_file);

  if (!errors.empty()) {
    std::cout << "Error parsing config file:" << std::endl;
    for (int i=1; i<=errors.size(); ++i) {
      std::cout << i << ". " << errors[i-1] << std::endl;
    }
    std::cout << "\x1b[?25h"; // show cursor
    exit(1);
  }


  // setup buffer
  auto [hei, wid] = core::get_terminal_size();
  for (auto& canvas : session::buffer) {
    canvas.resize(wid);
    for (auto& row : canvas) {
      row.resize(hei);
    }
  }

  std::thread input_thread(session::_input_thread);
  std::thread output_thread(session::_output_thread);
  std::thread compute_thread(session::_compute_thread, points, normals);

  input_thread.join();
  output_thread.join();
  compute_thread.join();

  session::terminal_mode_reset();

  config::serialize_config(parameter::cur_params, control::keymap, write_to);
}

int main(int argc, char** argv) {
  // test_terminal_size();
  // test_draw();

  // session::entry();
  // test_animate();

  // CLI::App app{"Donut CLI"};
  // int level = 0;
  // app.add_option("-l,--level", level, "Set animation level");
  // CLI11_PARSE(app, argc, argv);
  // return 0;

  // test_toml();
  
  test_termios();
}
