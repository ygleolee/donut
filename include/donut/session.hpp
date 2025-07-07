#pragma once

#include "donut/types.hpp"

#include <atomic>
#include <termios.h>
#include <unistd.h>

namespace donut::session {
extern std::atomic<bool> terminate;

void sigint_handler(int);

void terminal_mode_set();
void terminal_mode_reset();

void entry();

void _input_thread();
void _output_thread();
void _compute_thread(ves points, ves normals);

}
