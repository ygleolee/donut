#include "donut/core.hpp"
#include "donut/io.hpp"
#include "donut/session.hpp"
#include "donut/utils.hpp"

#include <unistd.h>
#include <thread>
#include <iostream>

namespace donut::core {

void draw(grd& canvas, ves& points, ves& normals, dbl viewer, vec light, donut::geometry::light_type light_src_type) {
  int n = points.size();
  int wid = canvas.size();
  int hei = canvas[0].size();
  grd depth(wid, std::vector<dbl>(hei, -1e6));

  for (int i=0; i<wid; ++i) {
    for (int j=0; j<hei; ++j) {
      canvas[i][j] = -1;
    }
  }

  if (light_src_type == donut::geometry::PARALLEL) { // treat 'light' as vector
    light = donut::geometry::neg(light);
  }

  for (int i=0; i<n; ++i) {
    vec pt = points[i];
    dbl scale = viewer / (viewer - pt[donut::geometry::Z]);
    int x_canvas = (int) round(scale * pt[donut::geometry::X] / donut::core::RANGE * (wid>>1) / donut::core::RATIO) + (wid>>1); // divide by ratio cuz the ratio of a character is hei/wid
    int y_canvas = (int) round(scale * pt[donut::geometry::Y] / donut::core::RANGE * (hei>>1)) + (hei>>1);
    if (!donut::utils::inrange(wid, hei, x_canvas, y_canvas)) continue;
    if (depth[x_canvas][y_canvas] > pt[donut::geometry::Z]) continue;
    depth[x_canvas][y_canvas] = pt[donut::geometry::Z];
    vec light_vec = light;
    if (light_src_type == donut::geometry::POINT) { // treat 'light' as light source
      light_vec = donut::geometry::diff(light, pt);
    }
    dbl brightness = (donut::geometry::cosang(light_vec, normals[i]) + 1) / 2; 
    canvas[x_canvas][y_canvas] = brightness;
  }

  // TODO: some angles cause a pixel that should be empty to have 3 non-empty neighbors, so the criteria doesn't work
  //       ideally, want the criteria to be neg <= 1 (cuz might be adjacent missed pixels)
  //       but even <= 0 cause issues somehow (when the middle hole becomes a cresent like shape)

  // for (int t=0; t<3; ++t) {
  //   for (int i=1; i<wid-1; ++i) { // only do [1, wid-1) to avoid out of bounds checks
  //     for (int j=1; j<hei-1; ++j) {
  //       int cnt = 0; // adjust if at least 2 neighbors are (significantly) different
  //       int neg = 0; // if more than 1 neighbors have negative depth, dont adjust (need shape to be convex?)
  //       dbl set = 0;
  //       for (int k=0; k<4; ++k) {
  //         int i_ = i + dx[k];
  //         int j_ = j + dy[k];
  //         if (canvas[i_][j_] < 0) { // dont adjust edges of the shape
  //           ++neg;
  //         }
  //         else if (canvas[i_][j_] - canvas[i][j] > THRESHOLD) {
  //           set += canvas[i_][j_];
  //           ++cnt;
  //         }
  //       }
  //       if (neg <= 0 && cnt >= 2) {
  //         canvas[i][j] = set / cnt;
  //       }
  //     }
  //   }
  // }
}

void rotate_shape(ves& points, ves& normals, vec degrees) {
  mat rot = donut::geometry::get_rotation_matrix(degrees);
  for (auto& pt : points) {
    pt = donut::geometry::apply(rot, pt);
  }
  for (auto& nor : normals) {
    nor = donut::geometry::apply(rot, nor);
  }
}

// TODO: testing code, remove later
void animate_simple(ves points, ves& normals, std::array<dbl, 3> degrees, dbl viewer, vec light, donut::geometry::light_type light_src_type, dbl interval) {
  int hei, wid;
  std::tie(hei, wid) = donut::utils::get_terminal_size();
  grd canvas(wid, std::vector<dbl>(hei));

  int n = donut::core::grayscale.size();
  while (true) {
    printf("\x1b[H");
    draw(canvas, points, normals, viewer, light, light_src_type);
    for (int j=hei-1; j>=0; --j) {
      for (int i=0; i<wid; ++i) {
        dbl brightness = canvas[i][j];
        if (brightness < 0) putchar_unlocked(' ');
        else putchar_unlocked(donut::core::grayscale[(int) (brightness * (n - 1))]);
      }
      putchar_unlocked('\n');
    }
    rotate_shape(points, normals, degrees);
    usleep(interval);
  }
}

void animate(ves points, ves normals) {
  std::cout << "\x1b[2J\x1b[H"; // clear screen
  std::cout << "\x1b[?25l";     // hide cursor

  int hei, wid;
  std::tie(hei, wid) = donut::utils::get_terminal_size();

  for (auto& canvas : donut::io::buffer) {
    canvas.resize(wid);
    for (auto& row : canvas) {
      row.resize(hei);
    }
  }

  std::thread compute_thread(donut::session::_compute_thread, points, normals);
  std::thread output_thread(donut::session::_output_thread);

  compute_thread.join();
  output_thread.join();
}

}
