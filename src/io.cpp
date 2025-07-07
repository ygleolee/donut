#include "donut/core.hpp"
#include "donut/io.hpp"
#include "donut/parameter.hpp"

#include <format>

namespace donut::io {

std::string move_cursor(int row, int col) {
  return std::format("\x1b[{};{}H", row, col);
}

void update_screen(grd& canvas, grd& old_canvas) {
  int wid = canvas.size();
  int hei = canvas[0].size();
  int n = donut::parameter::params.display.grayscale.size();

  std::string output = "\x1b[H";

  for (int j=hei-1; j>=0; --j) {
    for (int i=0; i<wid; ++i) {
      char cur =     canvas[i][j] < 0 ? ' ' : donut::parameter::params.display.grayscale[(int) (    canvas[i][j] * (n - 1))];
      char old = old_canvas[i][j] < 0 ? ' ' : donut::parameter::params.display.grayscale[(int) (old_canvas[i][j] * (n - 1))];
      if (cur != old) {
        output += move_cursor(hei - j, i + 1) + cur;
        old_canvas[i][j] = cur;
      }
    }
  }
  fwrite(output.data(), 1, output.size(), stdout);
  fflush(stdout);
}

}
