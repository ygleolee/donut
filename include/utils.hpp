#pragma once

#include <utility>
#include <array>

using dbl = double;

const std::array<int, 4> dx = { 0, 0, 1, -1 };
const std::array<int, 4> dy = { 1, -1, 0, 0 };

std::pair<int, int> get_terminal_size();

bool inrange(int n, int m, int i, int j);

