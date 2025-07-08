#pragma once

#include "donut/types.hpp"

#include <mutex>

// TODO: add randomness option for rps's
// other desireable controls
// one-time rotate the donut (apart from periodical rotation)

namespace donut::parameter {

extern std::mutex params_mtx;

struct light {
  light_type type;
  vec parallel;
  vec point;
  vec rps;
  // bool rand;
};

struct camera {
  dbl z;
  // TODO: add x, y, angle?
};

struct shape {
  vec rps;
  dbl delta; // change with one press
  // bool rand;
};

struct display {
  std::string grayscale;
  dbl range;
  dbl char_ratio;
  int fps;
};

struct params {
  struct light light;
  struct camera camera;
  struct shape shape;
  struct display display;
};

extern struct params cur_params;
extern struct params default_params;

}
