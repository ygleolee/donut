#pragma once

#include "donut/types.hpp"

#include <mutex>

namespace donut::parameter {

using light_params_t = struct {
  light_type type;
  vec parallel;
  vec point;
  vec rps;
  dbl rpp;
};

using camera_params_t = struct {
  dbl min;
  dbl max;
  int steps;
  vdb locs;
  int idx;
};

using shape_params_t = struct {
  vec rps;
  dbl delta; // change with one press
};

using display_params_t = struct {
  std::string grayscale;
  dbl range;
  dbl char_ratio;
  int fps;
};

// TODO: add these to config reading & handling
using animation_params_t = struct {
  int buffer_size;
  int fallback_keep;
};
using control_params_t = struct {
  int debounce;
};

using params_t = struct {
  light_params_t light;
  camera_params_t camera;
  shape_params_t shape;
  display_params_t display;
};

extern std::mutex params_mtx;

extern const params_t default_params;
extern params_t cur_params;

void try_setup_char_ratio(params_t& params);
void setup_camera_movement(params_t& params);
void read_config();

}
