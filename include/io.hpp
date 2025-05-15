#pragma once

#include "utils.hpp"

#include <string>
#include <vector>

const int CACHE_SIZE = 5;

std::string move_cursor(int row, int col);

// note: also directly updates old_canvas 
void output_to_screen(
  std::vector<std::vector<dbl>>& canvas,
  std::vector<std::vector<dbl>>& old_canvas
);

