#include "donut/io.hpp"
#include "donut/core.hpp"
#include "donut/interactive.hpp"

#include <format>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <chrono>

namespace donut::io {

std::mutex mtx;
std::condition_variable cv_compute;
std::condition_variable cv_output;
int buffer_cnt = 0;
std::array<std::vector<std::vector<dbl>>, donut::io::MAX_BUFFER_SIZE> buffer;

std::string move_cursor(int row, int col) {
  return std::format("\x1b[{};{}H", row, col);
}

void update_screen(std::vector<std::vector<dbl>>& canvas, std::vector<std::vector<dbl>>& old_canvas) {
  int wid = canvas.size();
  int hei = canvas[0].size();
  int n = donut::core::grayscale.size();

  std::string output = "\x1b[H";

  for (int j=hei-1; j>=0; --j) {
    for (int i=0; i<wid; ++i) {
      char cur =     canvas[i][j] < 0 ? ' ' : donut::core::grayscale[(int) (    canvas[i][j] * (n - 1))];
      char old = old_canvas[i][j] < 0 ? ' ' : donut::core::grayscale[(int) (old_canvas[i][j] * (n - 1))];
      if (cur != old) {
        output += move_cursor(hei - j, i + 1) + cur;
        old_canvas[i][j] = cur;
      }
    }
  }
  fwrite(output.data(), 1, output.size(), stdout);
  fflush(stdout);
}

void _compute_thread(std::vector<vec>& points, std::vector<vec>& normals) {
  int wid = buffer[0].size();
  int hei = buffer[0][0].size();
  std::vector<std::vector<dbl>> canvas(wid, std::vector<dbl>(hei, -1));

  for (int id = 0; ; id = (id + 1) % donut::io::MAX_BUFFER_SIZE) {
    {
      std::unique_lock<std::mutex> lock(mtx);
      cv_compute.wait(lock, [] {
        return buffer_cnt < donut::io::MAX_BUFFER_SIZE;
      });
    }

    donut::core::draw(canvas, points, normals, donut::interactive::viewer, (donut::interactive::light_src_type == donut::geometry::PARALLEL ? donut::interactive::light_parallel : donut::interactive::light_point), donut::interactive::light_src_type);
    donut::core::rotate_shape(points, normals, donut::interactive::rotation);

    {
      std::lock_guard<std::mutex> lock(mtx);
      buffer[id] = canvas;
      buffer_cnt += 1;
    }

    cv_output.notify_one();
  }
}

void _output_thread() {
  int wid = buffer[0].size();
  int hei = buffer[0][0].size();
  std::vector<std::vector<dbl>> canvas(wid, std::vector<dbl>(hei));
  std::vector<std::vector<dbl>> old_canvas(wid, std::vector<dbl>(hei, -1));

  for (int id = 0; ; id = (id + 1) % donut::io::MAX_BUFFER_SIZE) {
    {
      std::unique_lock<std::mutex> lock(mtx);
      cv_output.wait(lock, [] {
        return buffer_cnt > 0;
      });
    }

    {
      std::lock_guard<std::mutex> lock(mtx);
      canvas = buffer[id];
      buffer_cnt -= 1;
    }

    cv_compute.notify_one();

    update_screen(canvas, old_canvas);
    std::this_thread::sleep_for(std::chrono::milliseconds((int64_t) (1000 / donut::interactive::fps)));
  }
}

}
