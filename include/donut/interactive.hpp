#pragma once

#include "donut/geometry.hpp"
#include "donut/types.hpp"

namespace donut::interactive {
  extern dbl viewer;
  extern vec rotation;
  extern donut::geometry::light_type light_src_type;
  extern vec light_parallel;
  extern vec light_point;
  extern dbl fps; 
}

// other desireable controls
// one-time rotate the donut (apart from periodical rotation)

// input handlers go here (see GPT)
