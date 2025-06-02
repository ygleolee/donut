#pragma once

#include "utils.hpp"

#include <array>


using vec = std::array<dbl, 3>;
using mat = std::array<std::array<dbl, 3>, 3>;

enum axis {
  X_AXIS, Y_AXIS, Z_AXIS
};

enum light_type {
  PARALLEL, POINT
};

constexpr vec zero = { 0, 0, 0 };
constexpr mat Zero = { zero, zero, zero };
constexpr mat Iden = {{ {1, 0, 0}, {0, 1, 0}, {0, 0, 1} }};
constexpr int X = 0;
constexpr int Y = 1;
constexpr int Z = 2;

mat matmul(mat& A, mat& B);
vec apply(mat& A, vec& v);
vec rotate(vec& v, dbl ang, axis ax);
mat get_rotation_matrix(vec& degrees);
vec add(vec& u, vec& v);
vec diff(vec& u, vec& v);
vec neg(vec& v);
vec unit(vec& v);
dbl dot(vec& u, vec& v);
dbl norm2(vec& v);
dbl cosang(vec& u, vec& v);

