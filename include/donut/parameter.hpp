#pragma once

#include "donut/types.hpp"

#include <mutex>

namespace donut::parameter {

using params_light_t = struct light {
  light_type type;
  vec parallel;
  vec point;
  vec rps;
  dbl rpp;
};

using params_camera_t = struct camera {
  dbl min;
  dbl max;
  int steps;
  vdb locs;
  int idx;
};

using params_shape_t = struct shape {
  vec rps;
  dbl delta; // change with one press
};

using params_display_t = struct display {
  std::string grayscale;
  dbl range;
  dbl char_ratio;
  int fps;
};

using params_t = struct params {
  params_light_t light;
  params_camera_t camera;
  params_shape_t shape;
  params_display_t display;
};

extern std::mutex params_mtx;

extern const struct params default_params;
extern struct params cur_params;

void try_setup_char_ratio(struct params& params);
void setup_camera_movement(struct params& params);
void read_config();

}
