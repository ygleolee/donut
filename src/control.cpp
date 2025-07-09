#include "donut/control.hpp"

#include "donut/parameter.hpp"
#include "donut/session.hpp"
#include "donut/types.hpp"

#define X 0
#define Y 1
#define Z 2

// input handling
//   r: reset to default
//   0: rest to no rotation
//   xXyYzZ: increase/decrease rotation speed in that direction
//   ctrl-c: quit
//   space: start/pause
//   f: next frame (only when paused)
//   p: toggle parallel/point
//   TODO: wasdqe: controls light
//      - par: rotate about each axis
//      - pnt: move along direction
//   TODO: jk: move camera (see parameters.cpp for what to implement)
//   TODO: c: show config
//   TODO: h: command history


const int FALLBACK_KEEP = 2;

namespace donut::control {

void invalidate_computed_frames(int keep) {
  using namespace session;
  LOCK(idx_mtx, hist_mtx);
  int lead = compute_idx - output_idx;
  if (lead < keep) return;
  else compute_idx = output_idx + keep;
  buffer_cnt = compute_idx - output_idx;
  retrieve = true;
}

void handle_user_input(int chars, char buf[3]) {
  using namespace parameter;
  if (chars == 1) {
    switch (buf[0]) {
      case 'r': {
        {
          LOCK(params_mtx);
          cur_params = default_params;
        }
        invalidate_computed_frames(FALLBACK_KEEP);
        break;
      }
      case '0': {
        {
          LOCK(params_mtx);
          cur_params.shape.rps = { 0, 0, 0 };
        }
        invalidate_computed_frames(FALLBACK_KEEP);
        break;
      }
      case 'x': {
        {
          LOCK(params_mtx);
          cur_params.shape.rps[X] += cur_params.shape.delta;
        }
        invalidate_computed_frames(FALLBACK_KEEP);
        break;
      }
      case 'X': {
        {
          LOCK(params_mtx);
          cur_params.shape.rps[X] -= cur_params.shape.delta;
        }
        invalidate_computed_frames(FALLBACK_KEEP);
        break;
      }
      case 'y': {
        {
          LOCK(params_mtx);
          cur_params.shape.rps[Y] += cur_params.shape.delta;
        }
        invalidate_computed_frames(FALLBACK_KEEP);
        break;
      }
      case 'Y': {
        {
          LOCK(params_mtx);
          cur_params.shape.rps[Y] -= cur_params.shape.delta;
        }
        invalidate_computed_frames(FALLBACK_KEEP);
        break;
      }
      case 'z': {
        {
          LOCK(params_mtx);
          cur_params.shape.rps[Z] += cur_params.shape.delta;
        }
        invalidate_computed_frames(FALLBACK_KEEP);
        break;
      }
      case 'Z': {
        {
          LOCK(params_mtx);
          cur_params.shape.rps[Z] -= cur_params.shape.delta;
        }
        invalidate_computed_frames(FALLBACK_KEEP);
        break;
      }
      case ' ': {
        if (session::advance == 0) session::advance = -1;
        else session::advance = 0;
        break;
      }
      case 'f': {
        if (session::advance >= 0) {
          ++session::advance;
        }
        break;
      }
      case 'p': {
        {
          LOCK(params_mtx);
          cur_params.light.type = (cur_params.light.type == PARALLEL) ? POINT : PARALLEL;
        }
        invalidate_computed_frames(FALLBACK_KEEP);
        break;
      }
      case 'w': {
        {
          LOCK(params_mtx);
          if (cur_params.light.type == PARALLEL) {
          }
          else {
          }
        }
        invalidate_computed_frames(FALLBACK_KEEP);
        break;
      }
      case 'a': {
        LOCK(params_mtx);
        break;
      }
      case 's': {
        LOCK(params_mtx);
        break;
      }
      case 'd': {
        LOCK(params_mtx);
        break;
      }
      case 'q': {
        LOCK(params_mtx);
        break;
      }
      case 'e': {
        LOCK(params_mtx);
        break;
      }
      case 'j': {
        {
          LOCK(params_mtx);
          auto& cam = cur_params.camera;
          ++cam.idx;
          if (cam.idx == cam.steps) cam.idx = cam.steps - 1;
        }
        invalidate_computed_frames(FALLBACK_KEEP);
        break;
      }
      case 'k': {
        {
          LOCK(params_mtx);
          auto& cam = cur_params.camera;
          --cam.idx;
          if (cam.idx == -1) cam.idx = 0;
        }
        invalidate_computed_frames(FALLBACK_KEEP);
        break;
      }
    }
  } else if (chars == 3 && buf[0] == '\x1b' && buf[1] == '[') {
    switch (buf[2]) {
      case 'A': break;
      case 'B': break;
      case 'C': break;
      case 'D': break;
    }
  }
}

}
