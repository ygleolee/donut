#include "geometry.hpp"
#include "core.hpp"
#include "utils.hpp"
#include "shapes.hpp"

#include <iostream>

void test_terminal_size() {
  std::pair<int, int> ts = get_terminal_size();
  std::cout << ts.first << ' ' << ts.second << std::endl;
}

void test_animate_simple() {
  dbl r1 = 30, r2 = 60;
  std::vector<vec> points, normals;
  tie(points, normals) = donut(r1, r2);

  animate_simple(points, normals, {0.05, 0.1, 0.1}, 1000.0, {0, -1, -1}, PARALLEL, 50000);
  // animate_simple(points, normals, {0.05, 0.1, 0.1}, 1000.0, {2, -3, -2}, PARALLEL);
  // animate_simple(points, normals, {0.08, 0.02, 0.04}, 1000.0, {200, 100, 200}, POINT);
}

void test_animate() {
  dbl r1 = 30, r2 = 60;
  std::vector<vec> points, normals;
  tie(points, normals) = donut(r1, r2);

  animate(points, normals);
}

int main() {
  // test_terminal_size();
  // test_draw();
  // test_animate_simple();
  test_animate();



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
