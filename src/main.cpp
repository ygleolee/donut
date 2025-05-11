#include "geometry.hpp"
#include "core.hpp"
#include "utils.hpp"

#include <iostream>
#include <algorithm>

void test_terminal_size() {
  std::pair<int, int> ts = get_terminal_size();
  std::cout << ts.first << ' ' << ts.second << std::endl;
}

void test_draw() {
  dbl r1 = 15, r2 = 60;
  std::vector<vec> points, normals;
  for (dbl phi=0; phi<6.28; phi+=0.015) {
    dbl cp=cos(phi), sp=sin(phi);
    mat rot = {{ {cp, 0, -sp}, {0, 1, 0}, {sp, 0, cp} }};
    for (dbl the=0; the<6.28; the+=0.04) {
      dbl ct=cos(the), st=sin(the);
      vec pt = { r2 + r1 * ct, r1 * st, 0 };
      vec nor = { ct, st, 0 };
      pt = apply(rot, pt);
      nor = apply(rot, nor);
      pt = rotate(pt, 0.8, X_AXIS);
      nor = rotate(nor, 0.8, X_AXIS);
      pt = rotate(pt, 0.6, Y_AXIS);
      nor = rotate(nor, 0.6, Y_AXIS);
      pt = rotate(pt, 0.2, Z_AXIS);
      nor = rotate(nor, 0.2, Z_AXIS);
      points.push_back(pt);
      normals.push_back(nor);
    }
  }

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
  for (dbl phi=0; phi<6.28; phi+=0.015) {
    dbl cp=cos(phi), sp=sin(phi);
    mat rot = {{ {cp, 0, -sp}, {0, 1, 0}, {sp, 0, cp} }};
    for (dbl the=0; the<6.28; the+=0.04) {
      dbl ct=cos(the), st=sin(the);
      vec pt = { r2 + r1 * ct, r1 * st, 0 };
      vec nor = { ct, st, 0 };
      pt = apply(rot, pt);
      nor = apply(rot, nor);
      pt = rotate(pt, 0.8, X_AXIS);
      nor = rotate(nor, 0.8, X_AXIS);
      pt = rotate(pt, 0.6, Y_AXIS);
      nor = rotate(nor, 0.6, Y_AXIS);
      pt = rotate(pt, 0.2, Z_AXIS);
      nor = rotate(nor, 0.2, Z_AXIS);
      points.push_back(pt);
      normals.push_back(nor);
    }
  }
  animate(points, normals, 1000.0, {0, -1, -1}, PARALLEL);
}

int main() {
  test_animate();
}
