#pragma once

#include "donut/utils.hpp"
#include "donut/geometry.hpp"

#include <string>
#include <vector>

namespace donut::io {

constexpr int MAX_BUFFER_SIZE = 10;

// buffer between compute and output thread
extern std::array<std::vector<std::vector<dbl>>, donut::io::MAX_BUFFER_SIZE> buffer;

std::string move_cursor(int row, int col);

// note: directly updates old_canvas
void update_screen(
  std::vector<std::vector<dbl>>& canvas,
  std::vector<std::vector<dbl>>& old_canvas
);

void _compute_thread(
  std::vector<vec>& points,
  std::vector<vec>& normals
);

void _output_thread();

}
