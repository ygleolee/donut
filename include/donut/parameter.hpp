#pragma once

#include "donut/types.hpp"

#include <mutex>

namespace donut::parameter {

struct light {
  light_type type;
  vec parallel;
  vec point;
  vec rps;
  // bool rand;
};

struct camera {
  dbl z;
  dbl min;
  dbl max;
  int granularity;
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

extern std::mutex params_mtx;
extern struct params cur_params;
extern struct params default_params;

}
