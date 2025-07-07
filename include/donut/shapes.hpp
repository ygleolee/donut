#pragma once

#include "donut/types.hpp"
#include "donut/geometry.hpp"

// all "shape" functions return (points, normals)
// TODO: adjust number of points needed based on resolution? (if bottleneck is I/O, we can just calculate a bunch of points)


namespace donut::shapes {
// r1 = donut radius, r2 = tube radius
std::pair<ves, ves> donut(dbl r1, dbl r2);

// r1 = donut radius, r2 = sphere radius, n = # of spheres
std::pair<ves, ves> mister_donut(dbl r1, dbl r2, int n);

// r = radius, if ax==X, circle will be on YZ-plane
std::pair<ves, ves> circle(dbl r, donut::geometry::axis ax);

// width, height, length
std::pair<ves, ves> cuboid(dbl w, dbl h, dbl l);

// axes lengths (note put (r, r, r) to get sphere)
std::pair<ves, ves> ellipsoid(dbl a, dbl b, dbl c);

// r1 = carbon radius, r2 = hydrogen radius, r3 = bond radius, l = bond length
std::pair<ves, ves> methane(dbl r1, dbl r2, dbl r3, dbl l);

// TODO: cone, droplet, pill, ...
}
