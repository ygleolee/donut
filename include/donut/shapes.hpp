#pragma once

#include "donut/geometry.hpp"
#include "donut/types.hpp"

namespace donut::shapes {
// r1 = donut radius, r2 = tube radius
shape_data donut(dbl r1, dbl r2);

// r1 = donut radius, r2 = sphere radius, n = # of spheres
shape_data mister_donut(dbl r1, dbl r2, int n);

// r = radius, if ax==X, circle will be on YZ-plane
shape_data circle(dbl r, geometry::axis ax);

// width, height, length
shape_data cuboid(dbl w, dbl h, dbl l);

// axes lengths (note put (r, r, r) to get sphere)
shape_data ellipsoid(dbl a, dbl b, dbl c);

// r1 = carbon radius, r2 = hydrogen radius, r3 = bond radius, l = bond length
shape_data methane(dbl r1, dbl r2, dbl r3, dbl l);

}
