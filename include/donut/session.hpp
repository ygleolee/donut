#pragma once

#include "donut/types.hpp"

#include <atomic>
#include <mutex>
#include <unistd.h>

namespace donut::session {

const int BUFFER_SIZE = 20;

extern std::atomic<bool> terminate;
extern std::atomic<int> advance;

extern std::array<grd, BUFFER_SIZE> buffer;
extern std::mutex buffer_mtx;
extern int buffer_cnt;

extern std::mutex idx_mtx;
extern uint64_t compute_idx;
extern uint64_t output_idx;

extern std::mutex hist_mtx;
extern bool retrieve;

void sigint_handler(int);

void terminal_mode_set();
void terminal_mode_reset();

void entry();

void _input_thread();
void _output_thread();
void _compute_thread(ves points, ves normals);

}
