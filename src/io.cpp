#include "io.hpp"
#include "core.hpp"

#include <format>

std::string move_cursor(int row, int col) {
  return std::format("\x1b[{};{}H", row, col);
}

void output_to_screen(std::vector<std::vector<dbl>>& canvas, std::vector<std::vector<dbl>>& old_canvas) {
  int wid = canvas.size();
  int hei = canvas[0].size();
  int n = grayscale.size();

  std::string output = "\x1b[H";

  for (int j=hei-1; j>=0; --j) {
    for (int i=0; i<wid; ++i) {
      char cur = grayscale[(int) (    canvas[i][j] * (n - 1))];
      char old = grayscale[(int) (old_canvas[i][j] * (n - 1))];
      if (cur != old) {
        output += move_cursor(hei - j, i + 1) + cur;
        old_canvas[i][j] = cur;
      }
    }
  }

  fwrite(output.data(), 1, output.size(), stdout);
  fflush(stdout);
}

