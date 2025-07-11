#pragma once

#include "donut/types.hpp"

#include <mutex>

namespace donut::parameter {

struct light {
  light_type type;
  vec parallel;
  vec point;
  vec rps;
  dbl rpp;
  // bool rand;
};

struct camera {
  dbl min;
  dbl max;
  int steps;
  vdb locs;
  int idx;
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

extern const struct params default_params;
extern struct params cur_params;

void setup_char_ratio(struct params& params);
void setup_camera_movement(struct params& params);
void read_config();

}
