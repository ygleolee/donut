#include "geometry.hpp"
#include "core.hpp"
#include "utils.hpp"
#include "shapes.hpp"

#include <iostream>
#include <algorithm>

void test_terminal_size() {
  std::pair<int, int> ts = get_terminal_size();
  std::cout << ts.first << ' ' << ts.second << std::endl;
}

void test_draw() {
  dbl r1 = 15, r2 = 60;
  std::vector<vec> points, normals;
  tie(points, normals) = donut(r1, r2);

  int hei, wid, canvas_size;
  std::tie(hei, wid) = get_terminal_size();
  canvas_size = std::min(hei, wid);

  std::vector<std::vector<char>> canvas(canvas_size, std::vector<char>(canvas_size));
  draw(canvas, points, normals, 1000.0, {0, -1, -1}, PARALLEL);

  int pad_size_l = (wid - hei) >> 1;
  int pad_size_r = wid - pad_size_l - hei;
  std::string pad_l(pad_size_l, ' ');
  std::string pad_r(pad_size_r, ' ');
  for (int i=0; i<hei; ++i) {
    std::cout << pad_l;
    for (int j=0; j<hei; ++j) {
      putchar_unlocked(canvas[j][hei-i-1]);
    }
    std::cout << pad_r;
  }
}

void test_animate() {
  dbl r1 = 25, r2 = 60;
  std::vector<vec> points, normals;
  tie(points, normals) = donut(r1, r2);

  animate(points, normals, {0.04, 0.08, 0.02}, 1000.0, {0, -1, -1}, PARALLEL);
}

int main() {
  // test_terminal_size();
  // test_draw();
  test_animate();
}
