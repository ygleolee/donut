#include "donut/parameter.hpp"

// Available parameters
// range: borders of the real plane ([-range, range] x [-range, range])
// char_ratio: character height/width (TODO: somehow get this from the terminal)
// viewer: viewer Z value
// light_src_type: parallel or point
//   light_parallel: direction of the light (for parallel)
//   light_point: absolute coordinates of the light (for point)
// rotation: radian per sec (TODO: current implementation is not per sec)
// fps: frames per second of animation
// grayscale: ascii grayscale (TODO: the first half must all be . maybe add this automatically?)

#include <mutex>

namespace donut::parameter {

std::mutex params_mtx;

struct params default_params = {
  .light = {
    .type = PARALLEL,
    .parallel = { 0, -1, -1 },
    .point = { 100, 40, 300 },
    .rps = { 0, 0, 0 },
  },
  .camera = {
    .z = 1200.0,
  },
  .shape = {
    .rps = { 0.028, 0.044, 0.053 },
  },
  .display = {
    .grayscale = "......,,,-~:;=!*#$@",
    .range = 100.0,
    .char_ratio = 1.75,
    .fps = 40,
  },
};

struct params params = default_params;

}
