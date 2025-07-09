#include "donut/parameter.hpp"

// Available parameters
// display:
//    range: borders of the real plane ([-range, range] x [-range, range])
//    char_ratio: character height/width
//    fps: frames per second of animation
// camera:
//    z: camera z-coord
//    min/max: range the z value can be adjusted in
//    granularity: how many presses to divide [min, max] into (the spaces will be exponetially decaying)
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
    .z = 600.0,
    .min = 200,
    .max = 1000,
    .granularity = 12,
  },
  .shape = {
    .rps = { 0.38, 0.64, 0.73 },
    .delta = 0.2,
  },
  .display = {
    .grayscale = "......,,,-~:;=!*#$@",
    // .grayscale = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/|()1{}[]?-_+~<>i!lI;:,^. ",
    .range = 100.0,
    .char_ratio = 1.75,
    .fps = 30,
  },
};

struct params cur_params = default_params;

}
