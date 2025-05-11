#include "utils.hpp"

#include <sys/ioctl.h>
#include <unistd.h>

std::pair<int, int> get_terminal_size() {
  struct winsize w;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
    return std::make_pair(-1, -1);
  }
  return std::make_pair(w.ws_row, w.ws_col);
}

bool inrange(int n, int m, int i, int j) {
  return (i < n) && (i >= 0) && (j < m) && (j >= 0);
}

