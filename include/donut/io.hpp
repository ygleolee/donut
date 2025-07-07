#pragma once

#include "donut/geometry.hpp"
#include "donut/types.hpp"

#include <string>

namespace donut::io {

constexpr int MAX_BUFFER_SIZE = 20;

// buffer between compute and output thread
extern std::array<grd, donut::io::MAX_BUFFER_SIZE> buffer;

extern std::condition_variable cv_compute;
extern std::condition_variable cv_output;

std::string move_cursor(int row, int col);

// note: directly updates old_canvas
void update_screen(
  grd& canvas,
  grd& old_canvas
);

}
