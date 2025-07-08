#include "donut/geometry.hpp"

#include <cmath>

namespace donut::geometry {

mat matmul(mat& A, mat& B) {
  mat ans = geometry::Zero;
  for (int i=0; i<3; ++i)
    for (int j=0; j<3; ++j)
      for (int k=0; k<3; ++k)
        ans[i][j] += A[i][k] * B[k][j];
  return ans;
}

vec apply(mat& A, vec& v) {
  vec ans = geometry::zero;
  for (int i=0; i<3; ++i)
    for (int j=0; j<3; ++j)
      ans[i] += A[i][j] * v[j];
  return ans;
}

vec rotate(vec& v, dbl ang, geometry::axis ax) {
  mat rot = geometry::Zero;
  dbl sin_ang = sin(ang);
  dbl cos_ang = cos(ang);
  switch(ax) {
    case geometry::X_AXIS:
      rot = {{ { 1, 0, 0 }, { 0, cos_ang, -sin_ang }, { 0, sin_ang, cos_ang } }};
      break;
    case geometry::Y_AXIS:
      rot = {{ { cos_ang, 0, -sin_ang }, { 0, 1, 0 }, { sin_ang, 0, cos_ang } }};
      break;
    case geometry::Z_AXIS:
      rot = {{ { cos_ang, -sin_ang, 0 }, { sin_ang, cos_ang, 0 }, { 0, 0, 1 } }};
  }
  return apply(rot, v);
}

mat get_rotation_matrix(vec rads) {
  dbl cos_x = cos(rads[X]);
  dbl sin_x = sin(rads[X]);
  dbl cos_y = cos(rads[Y]);
  dbl sin_y = sin(rads[Y]);
  dbl cos_z = cos(rads[Z]);
  dbl sin_z = sin(rads[Z]);
  mat rot_x = {{ { 1, 0, 0 }, { 0, cos_x, -sin_x }, { 0, sin_x, cos_x } }};
  mat rot_y = {{ { cos_y, 0, sin_y }, { 0, 1, 0 }, { -sin_y, 0, cos_y } }};
  mat rot_z = {{ { cos_z, -sin_z, 0 }, { sin_z, cos_z, 0 }, { 0, 0, 1 } }};
  mat res = matmul(rot_x, rot_y);
  res = matmul(res, rot_z);
  return res;
}

vec add(vec& u, vec& v) {
  return { u[0] + v[0], u[1] + v[1], u[2] + v[2] };
}

vec diff(vec& u, vec& v) {
  return { u[0] - v[0], u[1] - v[1], u[2] - v[2] };
}

vec neg(vec& v) {
  vec res = { -v[0], -v[1], -v[2] };
  return res;
}

vec unit(vec& v) {
  dbl siz = sqrt(geometry::norm2(v));
  vec res = {v[0]/siz, v[1]/siz, v[2]/siz};
  return res;
}

dbl dot(vec& u, vec& v) {
  return u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
}

dbl norm2(vec& v) {
  return dot(v, v);
}

dbl cosang(vec& u, vec& v) {
  dbl dot = u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
  dbl u_norm2 = norm2(u);
  dbl v_norm2 = norm2(v);
  return dot / sqrt(u_norm2 * v_norm2);
}

}
