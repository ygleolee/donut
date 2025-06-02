#include "interactive.hpp"

dbl viewer = 1200.0;
// light_type light_src_type = PARALLEL;
light_type light_src_type = POINT;
vec light_parallel = { 1, -1, -1 };
vec light_point = { 100, 40, 300 };

// TODO: put these in configuration.cpp or something maybe cuz not directly controlled by user with the keyboard
vec rotation = { 0.028, 0.044, 0.053 }; // TODO: change this to per second rotation instead
dbl fps = 40;

// TODO: user can press a key (ex. S) to stop the animation somehow ...
