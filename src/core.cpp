#include "donut/core.hpp"

#include "donut/geometry.hpp"
#include "donut/parameter.hpp"

#include <algorithm>
#include <format>
#include <ranges>
#include <sys/ioctl.h>
#include <unistd.h>

#define OOB(n, m, i, j) ((i >= n) || (i < 0) || (j >= m) || (j < 0))
#define X 0
#define Y 1
#define Z 2

// TODO: config page, show (decide where to do this, core/control/parameter)
// show current rps, light src, light vec, grayscale, etc.
// real fps

namespace donut::core {

std::pair<int, int> get_terminal_size() {
  struct winsize w;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
    return { -1, -1 };
  }
  return { w.ws_row, w.ws_col };
}

std::string move_cursor(int row, int col) {
  return std::format("\x1b[{};{}H", row, col);
}

void update_screen(grd& canvas, grd& old_canvas) {
  static bool init = true;
  static int wid = 0;
  static int hei = 0;

  if (init) {
    std::scoped_lock<std::mutex> lock(parameter::params_mtx);
    wid = canvas.size();
    hei = canvas[0].size();
    init = false;
  }

  std::string output = "\x1b[H";
  for (int i = 0; i < wid; ++i) {
    for (int j = 0; j < hei; ++j) {
      char cur = canvas[i][j];
      char old = old_canvas[i][j];
      if (cur != old) {
        output += move_cursor(hei - j, i + 1) + cur;
        old_canvas[i][j] = canvas[i][j];
      }
    }
  }
  
  fwrite(output.data(), 1, output.size(), stdout);
  fflush(stdout);
}

void draw(grd& canvas, ves& points, ves& normals) {
  light_type type;
  vec light;
  dbl z;
  dbl range;
  dbl char_ratio;
  static bool init = true;
  static std::string grayscale = "";
  static int len = 0;
  static int wid = 0;
  static int hei = 0;
  static std::vector<std::vector<dbl>> depth;

  {
    using namespace parameter;
    std::scoped_lock<std::mutex> lock(params_mtx);
    type = cur_params.light.type;
    light = (type == PARALLEL) ? geometry::neg(cur_params.light.parallel) : cur_params.light.point;
    z = cur_params.camera.z;
    range = cur_params.display.range;
    char_ratio = cur_params.display.char_ratio;
    if (init) {
      grayscale = cur_params.display.grayscale;
      len = grayscale.size();
      init = false;
      wid = canvas.size();
      hei = canvas[0].size();
      depth.resize(wid);
      for (auto& row : depth) row.resize(hei);
    }
  }

  for (int i = 0; i < wid; ++i) {
    for (int j = 0; j < hei; ++j) {
      canvas[i][j] = ' ';
      depth[i][j] = -1e9;
    }
  }

  for (auto const &[pt, nor] : std::views::zip(points, normals)) {
    dbl scale = z / (z - pt[Z]);
    int x_canvas = (wid >> 1) + (int) round(scale * pt[X] / range * (wid >> 1) / char_ratio);
    int y_canvas = (hei >> 1) + (int) round(scale * pt[Y] / range * (hei >> 1));
    if (OOB(wid, hei, x_canvas, y_canvas) || depth[x_canvas][y_canvas] > pt[Z]) continue;

    vec light_vec = light;
    if (type == POINT) light_vec = geometry::diff(light, pt);
    dbl brightness = (geometry::cosang(light_vec, nor) + 1) / 2;  // [-1, 1] -> [0, 1]

    canvas[x_canvas][y_canvas] = grayscale[(int) (brightness * (len - 1))];
    depth[x_canvas][y_canvas] = pt[Z];
  }
}

void rotate_shape(ves& points, ves& normals, vec degrees) {
  mat rot = geometry::get_rotation_matrix(degrees);
  std::ranges::for_each(points, [&](vec& pt) {
    pt = geometry::apply(rot, pt);
  });
  std::ranges::for_each(normals, [&](vec& nor) {
    nor = geometry::apply(rot, nor);
  });
}

}
