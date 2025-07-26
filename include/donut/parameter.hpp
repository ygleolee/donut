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

using animation_params_t = struct {
  int buffer_size;
  int fallback_keep;
};
using control_params_t = struct {
  int debounce;
};

using mutable_params_t = struct {
  light_params_t light;
  camera_params_t camera;
  shape_params_t shape;
};

using immutable_params_t = struct {
  display_params_t display;
  animation_params_t animation;
  control_params_t control;
};


extern const mutable_params_t mutable_params_default;
extern const immutable_params_t immutable_params_default;

extern mutable_params_t mutable_params;
extern immutable_params_t immutable_params;

extern std::mutex params_mtx;

void try_setup_char_ratio(display_params_t& params);
void setup_camera_movement(camera_params_t& params);

}
