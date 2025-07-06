#pragma once

#include "donut/utils.hpp"
#include "donut/geometry.hpp"

#include <vector>

// all "shape" functions return (points, normals)
// TODO: adjust number of points needed based on resolution? (if bottleneck is I/O, we can just calculate a bunch of points)

// r1 = donut radius, r2 = tube radius
std::pair<std::vector<vec>, std::vector<vec>> donut(dbl r1, dbl r2);

// r1 = donut radius, r2 = sphere radius, n = # of spheres
std::pair<std::vector<vec>, std::vector<vec>> mister_donut(dbl r1, dbl r2, int n);

// r = radius, if ax==X, circle will be on YZ-plane
std::pair<std::vector<vec>, std::vector<vec>> circle(dbl r, axis ax);

// width, height, length
std::pair<std::vector<vec>, std::vector<vec>> cuboid(dbl w, dbl h, dbl l);

// axes lengths (note put (r, r, r) to get sphere)
std::pair<std::vector<vec>, std::vector<vec>> ellipsoid(dbl a, dbl b, dbl c);

// r1 = carbon radius, r2 = hydrogen radius, r3 = bond radius, l = bond length
std::pair<std::vector<vec>, std::vector<vec>> methane(dbl r1, dbl r2, dbl r3, dbl l);

// TODO: cone, droplet, pill, ...
