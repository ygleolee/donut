#include "interactive.hpp"

dbl viewer = 1000.0;
light_type light_src_type = PARALLEL;
vec light_parallel = { 0, -1, -1 };
vec light_point = { 50, 10, 30 };

// TODO: put these in configuration.cpp or something maybe cuz not directly controlled by user with the keyboard
vec rotation = { 0.04, 0.02, 0.08 };
dbl fps = 10;

// TODO: user can press a key (ex. S) to stop the animation somehow ...
