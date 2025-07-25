#include "donut/control.hpp"

#include "donut/geometry.hpp"
#include "donut/parameter.hpp"
#include "donut/session.hpp"
#include "donut/types.hpp"

#define X 0
#define Y 1
#define Z 2

// input handling
//   ctrl-c: quit
//   r: reset to default
//   0: rest to no rotation
//   xXyYzZ: increase/decrease rotation speed in that direction
//   wasdqe: controls light
//      - par: rotate about each axis
//      - pnt: rotate about each axis, stays on the same sphere
//   p: toggle parallel/point
//   space: start/pause
//   f: next frame (only when paused)
//   jk: move camera (see parameters.cpp for what to implement)
//   TODO: c: show config
//   TODO: h: command history


constexpr int FALLBACK_KEEP = 5;

namespace donut::control {

std::unordered_map<char, int> key_mappings;

void setup_default_keymap(std::unordered_map<char, int>& keymap) {
  keymap['r'] = RESET                ;
  keymap['0'] = ZERO_ROTATION        ;
  keymap['x'] = INCREASE_SHAPE_ROT_X ;
  keymap['X'] = DECREASE_SHAPE_ROT_X ;
  keymap['y'] = INCREASE_SHAPE_ROT_Y ;
  keymap['Y'] = DECREASE_SHAPE_ROT_Y ;
  keymap['z'] = INCREASE_SHAPE_ROT_Z ;
  keymap['Z'] = DECREASE_SHAPE_ROT_Z ;
  keymap['w'] = INCREASE_LIGHT_ROT_X ;
  keymap['s'] = DECREASE_LIGHT_ROT_X ;
  keymap['a'] = INCREASE_LIGHT_ROT_Y ;
  keymap['d'] = DECREASE_LIGHT_ROT_Y ;
  keymap['q'] = INCREASE_LIGHT_ROT_Z ;
  keymap['e'] = DECREASE_LIGHT_ROT_Z ;
  keymap['p'] = LIGHT_TOGGLE_TYPE    ;
  keymap[' '] = START_PAUSE          ;
  keymap['f'] = NEXT_FRAME           ;
  keymap['j'] = CAMERA_FORWARD       ;
  keymap['k'] = CAMERA_BACKWARD      ;
  keymap['c'] = SHOW_CONFIG          ;
  keymap['h'] = COMMAND_HISTORY      ;
}

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
    switch (key_mappings[buf[0]]) {
      case RESET: {
        {
          LOCK(params_mtx);
          cur_params = default_params;
          try_setup_char_ratio(cur_params);
          setup_camera_movement(cur_params);
        }
        invalidate_computed_frames(FALLBACK_KEEP);
        break;
      }
      case ZERO_ROTATION: {
        {
          LOCK(params_mtx);
          cur_params.shape.rps = { 0, 0, 0 };
        }
        invalidate_computed_frames(FALLBACK_KEEP);
        break;
      }
      case INCREASE_SHAPE_ROT_X: {
        {
          LOCK(params_mtx);
          cur_params.shape.rps[X] += cur_params.shape.delta;
        }
        invalidate_computed_frames(FALLBACK_KEEP);
        break;
      }
      case DECREASE_SHAPE_ROT_X: {
        {
          LOCK(params_mtx);
          cur_params.shape.rps[X] -= cur_params.shape.delta;
        }
        invalidate_computed_frames(FALLBACK_KEEP);
        break;
      }
      case INCREASE_SHAPE_ROT_Y: {
        {
          LOCK(params_mtx);
          cur_params.shape.rps[Y] += cur_params.shape.delta;
        }
        invalidate_computed_frames(FALLBACK_KEEP);
        break;
      }
      case DECREASE_SHAPE_ROT_Y: {
        {
          LOCK(params_mtx);
          cur_params.shape.rps[Y] -= cur_params.shape.delta;
        }
        invalidate_computed_frames(FALLBACK_KEEP);
        break;
      }
      case INCREASE_SHAPE_ROT_Z: {
        {
          LOCK(params_mtx);
          cur_params.shape.rps[Z] += cur_params.shape.delta;
        }
        invalidate_computed_frames(FALLBACK_KEEP);
        break;
      }
      case DECREASE_SHAPE_ROT_Z: {
        {
          LOCK(params_mtx);
          cur_params.shape.rps[Z] -= cur_params.shape.delta;
        }
        invalidate_computed_frames(FALLBACK_KEEP);
        break;
      }
      case START_PAUSE: {
        if (session::advance == 0) session::advance = -1;
        else session::advance = 0;
        break;
      }
      case NEXT_FRAME: {
        if (session::advance >= 0) {
          ++session::advance;
        }
        break;
      }
      case LIGHT_TOGGLE_TYPE: {
        {
          LOCK(params_mtx);
          cur_params.light.type = (cur_params.light.type == PARALLEL) ? POINT : PARALLEL;
        }
        invalidate_computed_frames(FALLBACK_KEEP);
        break;
      }
      case INCREASE_LIGHT_ROT_X: {
        {
          LOCK(params_mtx);
          struct parameter::light& light = cur_params.light;
          if (light.type == PARALLEL) light.parallel = geometry::rotate(light.parallel, light.rpp, geometry::X_AXIS);
          else light.point = geometry::rotate(light.point, light.rpp, geometry::X_AXIS);
        }
        invalidate_computed_frames(FALLBACK_KEEP);
        break;
      }
      case DECREASE_LIGHT_ROT_X: {
        {
          LOCK(params_mtx);
          struct parameter::light& light = cur_params.light;
          if (light.type == PARALLEL) light.parallel = geometry::rotate(light.parallel, -light.rpp, geometry::X_AXIS);
          else light.point = geometry::rotate(light.point, -light.rpp, geometry::X_AXIS);
        }
        invalidate_computed_frames(FALLBACK_KEEP);
        break;
      }
      case INCREASE_LIGHT_ROT_Y: {
        {
          LOCK(params_mtx);
          struct parameter::light& light = cur_params.light;
          if (light.type == PARALLEL) light.parallel = geometry::rotate(light.parallel, light.rpp, geometry::Y_AXIS);
          else light.point = geometry::rotate(light.point, light.rpp, geometry::Y_AXIS);
        }
        invalidate_computed_frames(FALLBACK_KEEP);
        break;
      }
      case DECREASE_LIGHT_ROT_Y: {
        {
          LOCK(params_mtx);
          struct parameter::light& light = cur_params.light;
          if (light.type == PARALLEL) light.parallel = geometry::rotate(light.parallel, -light.rpp, geometry::Y_AXIS);
          else light.point = geometry::rotate(light.point, -light.rpp, geometry::Y_AXIS);
        }
        invalidate_computed_frames(FALLBACK_KEEP);
        break;
      }
      case INCREASE_LIGHT_ROT_Z: {
        {
          LOCK(params_mtx);
          struct parameter::light& light = cur_params.light;
          if (light.type == PARALLEL) light.parallel = geometry::rotate(light.parallel, light.rpp, geometry::Z_AXIS);
          else light.point = geometry::rotate(light.point, light.rpp, geometry::Z_AXIS);
        }
        invalidate_computed_frames(FALLBACK_KEEP);
        break;
      }
      case DECREASE_LIGHT_ROT_Z: {
        {
          LOCK(params_mtx);
          struct parameter::light& light = cur_params.light;
          if (light.type == PARALLEL) light.parallel = geometry::rotate(light.parallel, -light.rpp, geometry::Z_AXIS);
          else light.point = geometry::rotate(light.point, -light.rpp, geometry::Z_AXIS);
        }
        invalidate_computed_frames(FALLBACK_KEEP);
        break;
      }
      case CAMERA_FORWARD: {
        {
          LOCK(params_mtx);
          auto& cam = cur_params.camera;
          ++cam.idx;
          if (cam.idx == cam.steps) cam.idx = cam.steps - 1;
        }
        invalidate_computed_frames(FALLBACK_KEEP);
        break;
      }
      case CAMERA_BACKWARD: {
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
  }
}

}
