#include "shapes.hpp"

#include <numbers>
 
std::pair<std::vector<vec>, std::vector<vec>> donut(dbl r1, dbl r2) {
  std::vector<vec> points, normals;
  dbl delta_phi = 0.02;
  dbl delta_the = 0.05;
  for (dbl phi=0; phi<6.28; phi+=delta_phi) {
    vec degrees = {0, phi, 0};
    mat rot = get_rotation_matrix(degrees);
    for (dbl the=0; the<6.28; the+=delta_the) {
      dbl ct = cos(the), st = sin(the);
      vec pt = { r1 + r2 * ct, r2 * st, 0 };
      vec nor = { ct, st, 0 };
      points.push_back(apply(rot, pt));
      normals.push_back(apply(rot, nor));
    }
  }
  return { points, normals };
}

std::pair<std::vector<vec>, std::vector<vec>> mister_donut(dbl r1, dbl r2, int n) {
  std::vector<vec> points, normals;
  dbl ang = 2 * std::numbers::pi / n;

  std::vector<vec> points_sphere, normals_sphere;
  std::tie(points_sphere, normals_sphere) = ellipsoid(r2, r2, r2);
  for (auto& pt:points_sphere) { // shift everything by {r1, 0, 0}
    pt[X] += r1;
  }

  for (int i=0; i<n; ++i) {
    vec degrees = { 0, 0, ang * i };
    mat rot = get_rotation_matrix(degrees);
    for (auto& pt:points_sphere) {
      points.push_back(apply(rot, pt));
    }
    for (auto& nor:normals_sphere) {
      normals.push_back(apply(rot, nor));
    }
  }
  return { points, normals };
}

std::pair<std::vector<vec>, std::vector<vec>> cuboid(dbl w, dbl h, dbl l) {
  std::vector<vec> points, normals;
  dbl inc = 0.5;
  for (dbl i=-w/2; i<=w/2; i+=inc) {
    for (dbl j=-h/2; j<=h/2; j+=inc) {
      points.push_back({i, j,  l/2}); normals.push_back({0, 0,  1});
      points.push_back({i, j, -l/2}); normals.push_back({0, 0, -1});
    }
  }
  for (dbl i=-h/2; i<=h/2; i+=inc) {
    for (dbl j=-l/2; j<=l/2; j+=inc) {
      points.push_back({ w/2, i, j}); normals.push_back({ 1, 0, 0});
      points.push_back({-w/2, i, j}); normals.push_back({-1, 0, 0});
    }
  }
  for (dbl i=-w/2; i<=w/2; i+=inc) {
    for (dbl j=-l/2; j<=l/2; j+=inc) {
      points.push_back({i,  h/2, j}); normals.push_back({0,  1, 0});
      points.push_back({i, -h/2, j}); normals.push_back({0, -1, 0});
    }
  }
  return { points, normals };
}

std::pair<std::vector<vec>, std::vector<vec>> ellipsoid(dbl a, dbl b, dbl c) {
  std::vector<vec> points, normals;
  dbl delta_the = 0.02;
  dbl delta_phi = 0.02;
  for (dbl the=0; the<2*std::numbers::pi; the+=delta_the) {
    dbl ct = cos(the), st = sin(the);
    for (dbl phi=0; phi<std::numbers::pi; phi+=delta_phi) {
      dbl cp = cos(phi), sp = sin(phi);
      points.push_back({ct*sp*a, st*sp*b, cp*c});  // standard ellipsoid parameterization
      normals.push_back({ct*sp/a, st*sp/b, cp/c}); // take gradient of the ellipsoid formula
    }
  }
  return { points, normals };
}

// std::pair<std::vector<vec>, std::vector<vec>> methane(dbl r1, dbl r2, dbl r3, dbl l) {
//   std::vector<vec> points, normals;
//   dbl ang = 109.5 / 180 * std::numbers::pi; // methane bond angle
//   dbl co = cos(ang), si = sin(ang), ct, st, cp, sp, x, y, z;
//   mat rot1 = {{ {co, -si, 0}, {si, co, 0}, {0, 0, 1} }}, rot2 = {{ {(-1.0)/2, 0, -sqrt(3)/2}, {0, 1, 0}, {sqrt(3)/2, 0, (-1.0)/2} }};
//   vec v={0, l, 0}, v1=apply(rot1, v), v2=apply(rot2, v1), v3=apply(rot2, v2), n, n1, n2, n3;
//   vec dis[4] = {v, v1, v2, v3};
//   for (dbl the=0; the<6.28; the+=0.02) {
//     ct=cos(the), st=sin(the);
//     for (dbl phi=0; phi<3.14; phi+=0.02) {
//       cp=cos(phi), sp=sin(phi), x=r2*ct*sp, y=r2*st*sp, z=r2*cp;
//       points.push_back({r1*ct*sp, r1*st*sp, r1*cp});
//       normals.push_back({r1*ct*sp, r1*st*sp, r1*cp});
//       for (int i=0; i<4; ++i) {
//         points.push_back({x+dis[i][0], y+dis[i][1], z+dis[i][2]});
//         normals.push_back({x, y, z});
//       }
//     }
//   }
//   for (dbl the=0; the<6.28; the+=0.02) {
//     co=cos(the), si=sin(the);
//     for (dbl h=r1; h<=l-r2; h+=0.3) {
//       v = {r3*co, h, r3*si};
//       n = {r3*co, 0, r3*si};
//       v1 = apply(rot1, v);
//       n1 = apply(rot1, n);
//       v2 = apply(rot2, v1);
//       n2 = apply(rot2, n1);
//       v3 = apply(rot2, v2);
//       n3 = apply(rot2, n2);
//       points.push_back(v);
//       points.push_back(v1);
//       points.push_back(v2);
//       points.push_back(v3);
//       normals.push_back(n);
//       normals.push_back(n1);
//       normals.push_back(n2);
//       normals.push_back(n3);
//     }
//   }
//   return { points, normals };
// }

