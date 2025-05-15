#include "io.hpp"

#include <format>

std::string move_cursor(int row, int col) {
  return std::format("\x1b[{};{}H", row, col);
}
