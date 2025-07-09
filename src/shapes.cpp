#include "donut/shapes.hpp"

#include "donut/core.hpp"

#include <cmath>
#include <numbers>

namespace donut::shapes {

using namespace geometry;

constexpr dbl PI = std::numbers::pi;
constexpr dbl TAU = 2 * PI;
constexpr dbl PIH = PI / 2;
 
shape_data donut(dbl r1, dbl r2) {
  dbl delta_phi = 0.01;
  dbl delta_the = 0.02;

  ves points, normals;
  points.reserve(TAU * TAU / delta_phi / delta_the);
  normals.reserve(TAU * TAU / delta_phi / delta_the);

  for (dbl phi = 0; phi < TAU; phi += delta_phi) {
    mat rot = get_rotation_matrix({0, phi, 0});
    for (dbl the = 0; the < TAU; the += delta_the) {
      dbl ct = cos(the), st = sin(the);
      vec pt = { r1 + r2 * ct, r2 * st, 0 };
      vec nor = { ct, st, 0 };
      points.push_back(apply(rot, pt));
      normals.push_back(apply(rot, nor));
    }
  }
  return { points, normals };
}

shape_data mister_donut(dbl r1, dbl r2, int n) {
  ves points, normals;
  dbl ang = TAU / n;

  ves points_sphere, normals_sphere;
  std::tie(points_sphere, normals_sphere) = ellipsoid(r2, r2, r2);
  for (auto& pt : points_sphere) { // shift everything by {r1, 0, 0}
    pt[X] += r1;
  }

  for (int i = 0; i < n; ++i) {
    mat rot = get_rotation_matrix({0, 0, ang * i});
    for (auto& pt : points_sphere) {
      points.push_back(apply(rot, pt));
    }
    for (auto& nor : normals_sphere) {
      normals.push_back(apply(rot, nor));
    }
  }
  return { points, normals };
}

shape_data circle(dbl r, axis ax) {
  ves points, normals;
  dbl delta_phi = 0.02;
  for (dbl phi = 0; phi < TAU; phi += delta_phi) {
    dbl co = cos(phi);
    dbl si = sin(phi);
    points.push_back({r*co, r*si, 0});
    normals.push_back({co, si, 0});
  }
  vec degrees = {0, 0, 0};
  if (ax == X_AXIS) degrees[X] = PIH;
  else if (ax == Y_AXIS) degrees[Y] = PIH;
  core::rotate_shape(points, normals, degrees);
  return { points, normals };
}

shape_data cuboid(dbl w, dbl h, dbl l) {
  ves points, normals;
  dbl inc = 0.5;
  for (dbl i = -w/2; i <= w/2; i += inc) {
    for (dbl j = -h/2; j <= h/2; j += inc) {
      points.push_back({i, j,  l/2}); normals.push_back({0, 0,  1});
      points.push_back({i, j, -l/2}); normals.push_back({0, 0, -1});
    }
  }
  for (dbl i = -h/2; i <= h/2; i += inc) {
    for (dbl j = -l/2; j <= l/2; j += inc) {
      points.push_back({ w/2, i, j}); normals.push_back({ 1, 0, 0});
      points.push_back({-w/2, i, j}); normals.push_back({-1, 0, 0});
    }
  }
  for (dbl i = -w/2; i <= w/2; i += inc) {
    for (dbl j = -l/2; j <= l/2; j += inc) {
      points.push_back({i,  h/2, j}); normals.push_back({0,  1, 0});
      points.push_back({i, -h/2, j}); normals.push_back({0, -1, 0});
    }
  }
  return { points, normals };
}

shape_data ellipsoid(dbl a, dbl b, dbl c) {
  ves points, normals;
  dbl delta_the = 0.02;
  dbl delta_phi = 0.02;
  for (dbl the = 0; the < TAU; the += delta_the) {
    dbl ct = cos(the), st = sin(the);
    for (dbl phi = 0; phi < TAU; phi += delta_phi) {
      dbl cp = cos(phi), sp = sin(phi);
      points.push_back({ct*sp*a, st*sp*b, cp*c});  // standard ellipsoid parameterization
      normals.push_back({ct*sp/a, st*sp/b, cp/c}); // take gradient of the ellipsoid formula
    }
  }
  return { points, normals };
}

shape_data methane(dbl r1, dbl r2, dbl r3, dbl l) {
  ves points, normals;
  dbl ang = 109.5 / 180 * PI; // methane bond angle
  mat rot1 = get_rotation_matrix({0, 0, ang}); // rotate about the z-axis by ang
  mat rot2 = get_rotation_matrix({0, TAU / 3, 0}); // rotate about the y-axis by 120 degrees

  // carbon atom (at (0, 0, 0) so no shift)
  tie(points, normals) = ellipsoid(r1, r1, r1);

  // hydrogen atoms
  std::array<vec, 4> hydrogens;
  hydrogens[0] = {0, l, 0}; // 1st hydrogen atom at {0, l, 0}
  hydrogens[1] = apply(rot1, hydrogens[0]); // 2nd hydrogen atom is h1 rotated by ang about the z-axis
  hydrogens[2] = apply(rot2, hydrogens[1]); // 3rd hydrogen atom is h2 rotated by 120 about the y-axis
  hydrogens[3] = apply(rot2, hydrogens[2]); // 4th hydrogen atom is h3 rotated by 120 about the y-axis

  ves points_hydrogen, normals_hydrogen;
  std::tie(points_hydrogen, normals_hydrogen) = ellipsoid(r2, r2, r2);

  for (int i = 0; i < 4; ++i) {
    for (auto& pt : points_hydrogen) {
      points.push_back(add(pt, hydrogens[i]));
    }
    for (auto& nor : normals_hydrogen) {
      normals.push_back(nor);
    }
  }

  // bonds
  ves points_circle, normals_circle;
  ves points_bond, normals_bond;
  std::tie(points_circle, normals_circle) = circle(r3, Y_AXIS);
  dbl delta_h = 0.5;
  for (dbl h = 0; h < l; h += delta_h) {
    vec shift = {0, h, 0};
    for (auto& pt : points_circle) {
      points_bond.push_back(add(pt, shift));
    }
    for (auto& nor : normals_circle) {
      normals_bond.push_back(nor);
    }
  }
  for (auto& pt : points_bond) {
    vec p1 = pt;
    vec p2 = apply(rot1, p1);
    vec p3 = apply(rot2, p2);
    vec p4 = apply(rot2, p3);
    points.push_back(p1);
    points.push_back(p2);
    points.push_back(p3);
    points.push_back(p4);
  }
  for (auto& nor : normals_bond) {
    vec n1 = nor;
    vec n2 = apply(rot1, n1);
    vec n3 = apply(rot2, n2);
    vec n4 = apply(rot2, n3);
    points.push_back(n1);
    points.push_back(n2);
    points.push_back(n3);
    points.push_back(n4);
  }

  return { points, normals };
}

}
