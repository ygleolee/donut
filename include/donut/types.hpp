#pragma once

#include <array>
#include <vector>

#define LOCK(...) std::scoped_lock lock(__VA_ARGS__)

using dbl = double;
using vec = std::array<dbl, 3>;
using ves = std::vector<vec>;
using mat = std::array<std::array<dbl, 3>, 3>;
using grd = std::vector<std::vector<char>>;
using vdb = std::vector<dbl>;
using shape_data = std::pair<ves, ves>;

enum light_type {
  PARALLEL, POINT
};
