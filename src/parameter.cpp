#include "donut/parameter.hpp"

#include <toml++/toml.hpp>

#include <algorithm>
#include <mutex>
#include <cstdlib>
#include <iostream>

// Available parameters
// display:
//    range: borders of the real plane ([-range, range] x [-range, range])
//    char_ratio: character height/width
//    fps: frames per second of animation
// camera:
//    z: camera z-coord
//    min/max: range the z value can be adjusted in
//    granularity: how many presses to divide [min, max] into (the spaces will be linearly decaying)
//    ...
// light:
//    type: parallel / point
//    parallel: vec
//    point: vec
//    ...
// shape:
//    rps: radian per sec
//    delta: num to increase/decrease for each key press
//    ...

namespace donut::parameter {

std::mutex params_mtx;

const struct params default_params = {
  .light = {
    .type = PARALLEL,
    .parallel = { 1, -4, -4 },
    .point = { 100, 40, 300 },
    .rps = { 0, 0, 0 },
    .rpp = 0.1,
  },
  .camera = {
    .min = 200.0,
    .max = 800.0,
    .steps = 12,
    .locs = vdb(12),
    .idx = 0,
  },
  .shape = {
    .rps = { 0.98, 0.64, 0.73 },
    .delta = 0.2,
  },
  .display = {
    .grayscale = ".......,,,--~~;;==**#$@ ",
    .range = 100.0,
    .char_ratio = 1.71,
    .fps = 30,
  },
};

struct params cur_params = default_params;

void setup_char_ratio(struct params& params) {
  const char* val = std::getenv("CHAR_RATIO");
  if (val != nullptr) {
    char* endptr = nullptr;
    double ratio = std::strtod(val, &endptr);
    errno = 0; // reset errno before call
    // Error checking
    if (endptr == val || errno == ERANGE || *endptr != '\0') {
      return;
    }
    params.display.char_ratio = ratio;
  }
}

// [min, min + 1, min + (1 + d), min + (1 + 2d), ..., max]
void setup_camera_movement(struct params& params) {
  struct camera& cam = params.camera;
  if (cam.max - cam.min < cam.steps || cam.steps <= 2) {
    cam = default_params.camera;
  }
  dbl min = cam.min;
  dbl max = cam.max;
  int steps = cam.steps;
  cam.locs.resize(steps);
  dbl d = 2 * (max - min - steps + 1) / ((steps - 1) * (steps - 2));
  cam.locs[0] = min;
  for (int i = 1; i < steps; ++i) {
    cam.locs[i] = cam.locs[i-1] + (d * (i - 1) + 1);
  }
  std::reverse(cam.locs.begin(), cam.locs.end());
  cam.idx = 0;
}

void read_config(struct params& params, std::string filename) {
  try {
    // shape params
    auto config = toml::parse_file(filename);

    // key mappings
  } catch (const toml::parse_error& err) {
    std::cerr << "Error parsing configuration file " << filename << ": " << err.description() << "\n  at " << err.source().begin << "\n";
    return;
  }
}

}
