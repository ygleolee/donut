#pragma once

#include "geometry.hpp"

#include <vector>

// all "shape" functions return (points, normals)
// TODO: adjust number of points needed based on resolution? (if bottleneck is I/O, we can just calculate a bunch of points)

std::pair<std::vector<vec>, std::vector<vec>> donut(dbl r1, dbl r2);
std::pair<std::vector<vec>, std::vector<vec>> mister_donut(dbl r1, dbl r2, int n);
