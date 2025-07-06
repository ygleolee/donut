#pragma once

#include "donut/utils.hpp"
#include "donut/geometry.hpp"

#include <string>
#include <vector>

constexpr int MAX_BUFFER_SIZE = 10;

// global variables defined in interactive.cpp for users to control
extern dbl viewer;
extern vec rotation;
extern light_type light_src_type;
extern vec light_parallel;
extern vec light_point;
extern dbl fps; 


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
