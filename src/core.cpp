#include "core.hpp"
#include "utils.hpp"

#include <iostream>
#include <unistd.h>

void draw(std::vector<std::vector<char>>& canvas, std::vector<vec>& points, std::vector<vec>& normals, dbl viewer, vec light, light_type light_src_type) {
  int n = points.size();
  int canvas_size = canvas.size();
  int canvas_range = canvas_size >> 1;
  std::vector<std::vector<dbl>> depth(canvas_size, std::vector<dbl>(canvas_size, -1e6));

  // clear canvas
  for (int i=0; i<canvas_size; ++i) {
    for (int j=0; j<canvas_size; ++j) {
      canvas[i][j] = ' ';
    }
  }

  if (light_src_type == PARALLEL) { // treat 'light' as vector
    light = neg(light);
  }

  for (int i=0; i<n; ++i) {
    dbl scale = viewer / (viewer - points[i][Z]);
    int x_canvas = (int) round(scale * points[i][X] / RANGE * canvas_range) + canvas_range;
    int y_canvas = (int) round(scale * points[i][Y] / RANGE * canvas_range) + canvas_range;
    if (!inrange(canvas_size, canvas_size, x_canvas, y_canvas)) continue;
    if (depth[x_canvas][y_canvas] > points[i][Z]) continue;
    depth[x_canvas][y_canvas] = points[i][Z];
    vec light_vec = light;
    if (light_src_type == POINT) { // treat 'light' as light source
      light_vec = diff(light, points[i]);
    }
    dbl brightness = (cosang(light_vec, normals[i]) + 1) / 2; 
    canvas[x_canvas][y_canvas] = grayscale[(int) (brightness * (grayscale.size() - 1))];
  }
}

void rotate(std::vector<vec>& points, std::vector<vec>& normals, vec degrees) {
  mat rot = get_rotation_matrix(degrees);
  for (auto& pt : points) {
    pt = apply(rot, pt);
  }
  for (auto& nor : normals) {
    nor = apply(rot, nor);
  }
}

void animate(std::vector<vec> points, std::vector<vec>& normals, dbl viewer, vec light, light_type light_src_type) {
  int hei, wid, canvas_size;
  std::tie(hei, wid) = get_terminal_size();
  canvas_size = std::min(hei, wid);

  std::vector<std::vector<char>> canvas(canvas_size, std::vector<char>(canvas_size));
  int pad_size_l = (wid - hei) >> 1;
  int pad_size_r = wid - pad_size_l - hei;
  std::string pad_l(pad_size_l, ' ');
  std::string pad_r(pad_size_r, ' ');

  vec degrees = { 0.08, 0.02, 0.04 };

  while (true) {
    printf("\x1b[H");
    draw(canvas, points, normals, 1000.0, {0, -1, -1}, PARALLEL);
    for (int i=0; i<hei; ++i) {
      std::cout << pad_l;
      for (int j=0; j<hei; ++j) {
        putchar_unlocked(canvas[j][hei-i-1]);
      }
      std::cout << pad_r << '\n';
    }
    rotate(points, normals, degrees);
    usleep(50000);
  }
}

