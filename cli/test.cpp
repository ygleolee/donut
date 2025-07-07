#include "donut/session.hpp"
#include "donut/core.hpp"
#include "donut/geometry.hpp"
#include "donut/shapes.hpp"
#include "donut/utils.hpp"

#include <iostream>
#include <thread>

void test_terminal_size() {
  std::pair<int, int> ts = donut::utils::get_terminal_size();
  std::cout << ts.first << ' ' << ts.second << std::endl;
}

void test_animate_simple() {
  dbl r1 = 30, r2 = 60;
  ves points, normals;
  tie(points, normals) = donut::shapes::donut(r1, r2);

  // donut::core::animate_simple(points, normals, {0.05, 0.1, 0.1}, 1000.0, {0, -1, -1}, donut::geometry::PARALLEL, 50000);
  // animate_simple(points, normals, {0.05, 0.1, 0.1}, 1000.0, {2, -3, -2}, PARALLEL);
  // animate_simple(points, normals, {0.08, 0.02, 0.04}, 1000.0, {200, 100, 200}, POINT);
}

void test_animate() {
  dbl r1 = 60, r2 = 30;
  dbl w = 50, h = 80, l = 40;
  ves points, normals;
  tie(points, normals) = donut::shapes::donut(r1, r2);
  // tie(points, normals) = ellipsoid(w, h, l);
  // tie(points, normals) = mister_donut(60, 24, 8);
  // tie(points, normals) = methane(24, 12, 4, 85);

  donut::core::animate(points, normals);
}

void test_termios() {
  donut::session::terminal_mode_set();
  std::thread input_thread(donut::session::_input_thread);
  input_thread.join();
  donut::session::terminal_mode_reset();
}

int main() {
  // test_terminal_size();
  // test_draw();
  // test_animate_simple();

  // donut::session::entry();
  test_animate();

  // test_termios();

  // TODO:
  // usage like "donut --mister_donut --rotate 0.04 0.01 0.5 ..."
  //            "donut" (this should draw the classic donut !!)
  //            "donut --cube --interactive"
  // list of things users can change
  //   animation related:
  //     rotation
  //     viewer
  //     light src_type (toggle)
  //     light direction / location (depending on type)
  //   others (might need to change to not const):
  //     CACHE_SIZE
  //     grayscale
  //    
}
