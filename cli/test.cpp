#include "donut/donut.hpp"

#include <iostream>
#include <thread>
#include <csignal>

#include <CLI/CLI.hpp>
#include <toml++/toml.hpp>

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
  donut::parameter::setup_char_ratio(donut::parameter::cur_params);
  donut::parameter::setup_camera_movement(donut::parameter::cur_params);
  donut::control::setup_default_keymap(donut::control::key_mappings);

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

void test_toml() {
  try {
    auto config = toml::parse_file("/Users/leo/Desktop/code/projects/donut/tmp/test.toml");
 
    std::cout << "Title: " << *config["title"].value<std::string>() << "\n";
    std::cout << "Width: " << *config["width"].value<int>() << "\n";
    std::cout << "Height: " << *config["height"].value<int>() << "\n";
    std::cout << "Fullscreen: " << (*config["fullscreen"].value<bool>() ? "true" : "false") << "\n";
 
    auto& anim = *config["animation"].as_table();
    std::cout << "FPS: " << *anim["fps"].value<int>() << "\n";
    std::cout << "Speed: " << *anim["speed"].value<double>() << "\n";
 
    auto& light = *config["light"].as_table();
    std::cout << "Light Type: " << *light["type"].value<std::string>() << "\n";
    std::cout << "Intensity: " << *light["intensity"].value<double>() << "\n";
  } catch (const toml::parse_error& err) {
    std::cerr << "Parse error: " << err.description() << "\n  at " << err.source().begin << "\n";
  }
}


int main(int argc, char** argv) {
  // test_terminal_size();
  // test_draw();

  // donut::session::entry();
  // test_animate();

  // CLI::App app{"Donut CLI"};
  // int level = 0;
  // app.add_option("-l,--level", level, "Set animation level");
  // CLI11_PARSE(app, argc, argv);
  // return 0;

  // test_toml();
  
  test_termios();
}
