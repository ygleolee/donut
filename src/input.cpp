#include "donut/input.hpp"

#include "donut/parameter.hpp"
#include "donut/session.hpp"

#define X 0
#define Y 1
#define Z 2

namespace donut::input {
  void handle_user_input(int read, char buf[3]) {
    using namespace parameter;
    if (read == 1) {
      switch (buf[0]) {
        case 'x': {
          std::scoped_lock<std::mutex> lock(params_mtx);
          cur_params.shape.rps[X] += cur_params.shape.delta;
          break;
        }
        case 'X': {
          std::scoped_lock<std::mutex> lock(params_mtx);
          cur_params.shape.rps[X] -= cur_params.shape.delta;
          break;
        }
        case 'y': {
          std::scoped_lock<std::mutex> lock(params_mtx);
          cur_params.shape.rps[Y] += cur_params.shape.delta;
          break;
        }
        case 'Y': {
          std::scoped_lock<std::mutex> lock(params_mtx);
          cur_params.shape.rps[Y] -= cur_params.shape.delta;
          break;
        }
        case 'z': {
          std::scoped_lock<std::mutex> lock(params_mtx);
          cur_params.shape.rps[Z] += cur_params.shape.delta;
          break;
        }
        case 'Z': {
          std::scoped_lock<std::mutex> lock(params_mtx);
          cur_params.shape.rps[Z] -= cur_params.shape.delta;
          break;
        }
        case ' ': {
          if (session::advance == 0) session::advance = -1;
          else session::advance = 0;
          break;
        }
        case 'f': {
          ++session::advance;
          break;
        }
        case 'w': {
          std::scoped_lock<std::mutex> lock(params_mtx);
          break;
        }
        case 'a': {
          std::scoped_lock<std::mutex> lock(params_mtx);
          break;
        }
        case 's': {
          std::scoped_lock<std::mutex> lock(params_mtx);
          break;
        }
        case 'd': {
          std::scoped_lock<std::mutex> lock(params_mtx);
          break;
        }
        case 'q': {
          std::scoped_lock<std::mutex> lock(params_mtx);
          break;
        }
        case 'e': {
          std::scoped_lock<std::mutex> lock(params_mtx);
          break;
        }
        case 'h': {
          std::scoped_lock<std::mutex> lock(params_mtx);
          break;
        }
        case 'j': {
          std::scoped_lock<std::mutex> lock(params_mtx);
          break;
        }
        case 'k': {
          std::scoped_lock<std::mutex> lock(params_mtx);
          break;
        }
        case 'l': {
          std::scoped_lock<std::mutex> lock(params_mtx);
          break;
        }
      }
    } else if (read == 3 && buf[0] == '\x1b' && buf[1] == '[') {
      switch (buf[2]) {
        case 'A': break;
        case 'B': break;
        case 'C': break;
        case 'D': break;
      }
    }
  }
}
