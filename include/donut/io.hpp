#pragma once

#include "donut/geometry.hpp"
#include "donut/types.hpp"

#include <condition_variable>
#include <string>

namespace donut::io {

extern std::condition_variable cv_compute;
extern std::condition_variable cv_output;

std::string move_cursor(int row, int col);

// note: directly updates old_canvas
void update_screen(
  grd& canvas,
  grd& old_canvas
);

}
