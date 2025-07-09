#pragma once

#include "donut/types.hpp"

// key ideas:
// 1. monitor is XY-plane, Z-axis comes out of screen, origin at center of screen.
// 2. the object is always located in the square [-range, range] x [-range, range]
// 3. 'canvas' is the screen (XY-plane) divided into pixels, depending on the resolution.

namespace donut::core {

std::string move_cursor(int row, int col);
void update_screen(grd& canvas, grd& old_canvas);
std::pair<int, int> get_terminal_size();
void draw(grd& canvas, ves& points, ves& normals);
void rotate_shape(ves& points, ves& normals, vec degrees);

}
