#include "core.hpp"
#include "utils.hpp"

#include <iostream>
#include <unistd.h>

void draw(std::vector<std::vector<char>>& canvas, std::vector<vec>& points, std::vector<vec>& normals, dbl viewer, vec light, light_type light_src_type) {
  int n = points.size();
  int wid = canvas.size();
  int hei = canvas[0].size();
  std::vector<std::vector<dbl>> depth(wid, std::vector<dbl>(hei, -1e6));

  // clear canvas
  for (int i=0; i<wid; ++i) {
    for (int j=0; j<hei; ++j) {
      canvas[i][j] = ' ';
    }
  }

  if (light_src_type == PARALLEL) { // treat 'light' as vector
    light = neg(light);
  }

  // int hei, wid;
  // std::tie(hei, wid) = get_terminal_size();
  // dbl ratio = ((dbl) hei) / ((dbl) wid);
  dbl ratio = 1.85; // character height/width (TODO: somehow get this from the terminal)

  for (int i=0; i<n; ++i) {
    dbl scale = viewer / (viewer - points[i][Z]);
    int x_canvas = (int) round(scale * points[i][X] / RANGE * (wid>>1) / ratio) + (wid>>1); // divide by ratio cuz the ratio of a character is hei/wid = ratio
    int y_canvas = (int) round(scale * points[i][Y] / RANGE * (hei>>1)) + (hei>>1);
    if (!inrange(wid, hei, x_canvas, y_canvas)) continue;
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

void animate(std::vector<vec> points, std::vector<vec>& normals, std::array<dbl, 3> degrees, dbl viewer, vec light, light_type light_src_type) {
  int hei, wid;
  std::tie(hei, wid) = get_terminal_size();

  std::vector<std::vector<char>> canvas(wid, std::vector<char>(hei));
  int pad_size_l = (wid - hei) >> 1;
  int pad_size_r = wid - pad_size_l - hei;
  std::string pad_l(pad_size_l, ' ');
  std::string pad_r(pad_size_r, ' ');

  while (true) {
    printf("\x1b[H");
    draw(canvas, points, normals, viewer, light, light_src_type);
    for (int i=0; i<hei; ++i) {
      // std::cout << pad_l;
      for (int j=0; j<wid; ++j) {
        putchar_unlocked(canvas[j][hei - 1 -i]);
      }
      // std::cout << pad_r << '\n';
      putchar_unlocked('\n');
    }
    rotate(points, normals, degrees);
    usleep(50000);
  }
}

