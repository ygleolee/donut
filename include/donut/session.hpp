#pragma once

#include "donut/types.hpp"

#include <atomic>
#include <unistd.h>

namespace donut::session {

const int MAX_BUFFER_SIZE = 20;

extern std::atomic<bool> terminate;
extern std::atomic<int> advance;
extern std::atomic<int> compute_idx;
extern std::atomic<int> output_idx;
extern std::array<grd, MAX_BUFFER_SIZE> buffer;

void sigint_handler(int);

void terminal_mode_set();
void terminal_mode_reset();

void entry();

void _input_thread();
void _output_thread();
void _compute_thread(ves points, ves normals);

}
