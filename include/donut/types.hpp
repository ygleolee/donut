#pragma once

#include <array>
#include <vector>

#define CONCAT_IMPL(a, b) a##b
#define CONCAT(a, b) CONCAT_IMPL(a, b)
#define LOCK(...) \
    std::scoped_lock CONCAT(_lock_, __LINE__)(__VA_ARGS__)
#define COND_LOCK(cond, mtx) \
    std::unique_lock<std::mutex> CONCAT(_lock_, __LINE__){mtx, std::defer_lock}; \
    if (cond) CONCAT(_lock_, __LINE__).lock();

using dbl = double;
using vec = std::array<dbl, 3>;
using ves = std::vector<vec>;
using mat = std::array<std::array<dbl, 3>, 3>;
using grd = std::vector<std::vector<char>>;
using vdb = std::vector<dbl>;
using vst = std::vector<std::string>;
using shape_data = std::pair<ves, ves>;

enum light_type {
  PARALLEL, POINT
};
