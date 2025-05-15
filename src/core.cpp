#include "core.hpp"
#include "utils.hpp"

#include <unistd.h>

void draw(std::vector<std::vector<dbl>>& canvas, std::vector<vec>& points, std::vector<vec>& normals, dbl viewer, vec light, light_type light_src_type) {
  int n = points.size();
  int wid = canvas.size();
  int hei = canvas[0].size();
  std::vector<std::vector<dbl>> depth(wid, std::vector<dbl>(hei, -1e6));

  // clear canvas
  for (int i=0; i<wid; ++i) {
    for (int j=0; j<hei; ++j) {
      canvas[i][j] = -1;
    }
  }

  if (light_src_type == PARALLEL) { // treat 'light' as vector
    light = neg(light);
  }

  for (int i=0; i<n; ++i) {
    vec pt = points[i];
    dbl scale = viewer / (viewer - pt[Z]);
    int x_canvas = (int) round(scale * pt[X] / RANGE * (wid>>1) / RATIO) + (wid>>1); // divide by ratio cuz the ratio of a character is hei/wid
    int y_canvas = (int) round(scale * pt[Y] / RANGE * (hei>>1)) + (hei>>1);
    if (!inrange(wid, hei, x_canvas, y_canvas)) continue;
    if (depth[x_canvas][y_canvas] > pt[Z]) continue;
    depth[x_canvas][y_canvas] = pt[Z];
    vec light_vec = light;
    if (light_src_type == POINT) { // treat 'light' as light source
      light_vec = diff(light, pt);
    }
    dbl brightness = (cosang(light_vec, normals[i]) + 1) / 2; 
    canvas[x_canvas][y_canvas] = brightness;
  }

  // for (int t=0; t<5; ++t) {
  //   for (int i=0; i<wid; ++i) {
  //     for (int j=0; j<hei; ++j) {
  //       if (canvas[i][j] > 0.5) continue;
  //       bool adjust = true;
  //       int cnt = 4; // adjust if at least 2 neighbors are (significantly) different
  //       dbl set = 0;
  //       for (int k=0; k<4; ++k) {
  //         int i_ = i + dx[k];
  //         int j_ = j + dy[k];
  //         if ((!inrange(wid, hei, i_, j_)) || (canvas[i_][j_] < 0)) {
  //           adjust = false;
  //           break;
  //         }
  //         if (canvas[i_][j_] < 0.5) {
  //           --cnt;
  //         }
  //         set = fmax(set, canvas[i_][j_]);
  //       }
  //       if (adjust && cnt >= 2) {
  //         canvas[i][j] = set;
  //         // canvas[i][j] = 1;
  //       }
  //     }
  //   }
  // }
}

void rotate_shape(std::vector<vec>& points, std::vector<vec>& normals, vec degrees) {
  mat rot = get_rotation_matrix(degrees);
  for (auto& pt : points) {
    pt = apply(rot, pt);
  }
  for (auto& nor : normals) {
    nor = apply(rot, nor);
  }
}

void animate_simple(std::vector<vec> points, std::vector<vec>& normals, std::array<dbl, 3> degrees, dbl viewer, vec light, light_type light_src_type, dbl interval) {
  int hei, wid;
  std::tie(hei, wid) = get_terminal_size();
  std::vector<std::vector<dbl>> canvas(wid, std::vector<dbl>(hei));

  int n = grayscale.size();
  while (true) {
    printf("\x1b[H");
    draw(canvas, points, normals, viewer, light, light_src_type);
    // for (int i=0; i<hei; ++i) {
    //   for (int j=0; j<wid; ++j) {
    for (int j=hei-1; j>=0; --j) {
      for (int i=0; i<wid; ++i) {
        // dbl brightness = canvas[j][hei - 1 - i];
        dbl brightness = canvas[i][j];
        if (brightness < 0) {
          putchar_unlocked(' ');
        }
        else {
          putchar_unlocked(grayscale[(int) (brightness * (n - 1))]);
        }
      }
      putchar_unlocked('\n');
    }
    rotate_shape(points, normals, degrees);
    usleep(interval);
  }
}

void animate(std::vector<vec> points, std::vector<vec>& normals) {
  // create producer and consumer threads and handle them
  // producer should keep cache (queue) at size CACHE
  // consumer should handle the FPS part and call output_to_screen()
  viewer += 1;
}

