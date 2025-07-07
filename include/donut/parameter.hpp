#pragma once

#include "donut/geometry.hpp"
#include "donut/types.hpp"

// TODO: add randomness option for rps's

namespace donut::parameter {

struct light {
  donut::geometry::light_type type;
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
  // bool rand;
};

struct display {
  std::string grayscale;
  dbl range;
  dbl char_ratio;
  dbl fps;
};

struct params {
  struct light light;
  struct camera camera;
  struct shape shape;
  struct display display;
};

extern struct params params;

}

// other desireable controls
// one-time rotate the donut (apart from periodical rotation)
