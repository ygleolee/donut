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
//   TODO: h: command history
//   TODO: C: show config
//   TODO: E: export config


namespace donut::control {

const std::unordered_map<std::string, operations> operations_map = {
  { "RESET_TO_DEFAULT", RESET_TO_DEFAULT },
  { "ZERO_ROTATION", ZERO_ROTATION },
  { "INC_SHAPE_ROT_X", INC_SHAPE_ROT_X },
  { "DEC_SHAPE_ROT_X", DEC_SHAPE_ROT_X },
  { "INC_SHAPE_ROT_Y", INC_SHAPE_ROT_Y },
  { "DEC_SHAPE_ROT_Y", DEC_SHAPE_ROT_Y },
  { "INC_SHAPE_ROT_Z", INC_SHAPE_ROT_Z },
  { "DEC_SHAPE_ROT_Z", DEC_SHAPE_ROT_Z },
  { "INC_LIGHT_ROT_X", INC_LIGHT_ROT_X },
  { "DEC_LIGHT_ROT_X", DEC_LIGHT_ROT_X },
  { "INC_LIGHT_ROT_Y", INC_LIGHT_ROT_Y },
  { "DEC_LIGHT_ROT_Y", DEC_LIGHT_ROT_Y },
  { "INC_LIGHT_ROT_Z", INC_LIGHT_ROT_Z },
  { "DEC_LIGHT_ROT_Z", DEC_LIGHT_ROT_Z },
  { "LIGHT_TOGGLE_TYPE", LIGHT_TOGGLE_TYPE },
  { "START_PAUSE", START_PAUSE },
  { "NEXT_FRAME", NEXT_FRAME },
  { "CAMERA_FORWARD", CAMERA_FORWARD },
  { "CAMERA_BACKWARD", CAMERA_BACKWARD },
  { "COMMAND_HISTORY", COMMAND_HISTORY },
  { "SHOW_CONFIG", SHOW_CONFIG },
  { "EXPORT_CONFIG", EXPORT_CONFIG },
};

const std::unordered_map<control::operations, std::string> reverse_operations_map = {
  { RESET_TO_DEFAULT, "RESET_TO_DEFAULT" },
  { ZERO_ROTATION, "ZERO_ROTATION" },
  { INC_SHAPE_ROT_X, "INC_SHAPE_ROT_X" },
  { DEC_SHAPE_ROT_X, "DEC_SHAPE_ROT_X" },
  { INC_SHAPE_ROT_Y, "INC_SHAPE_ROT_Y" },
  { DEC_SHAPE_ROT_Y, "DEC_SHAPE_ROT_Y" },
  { INC_SHAPE_ROT_Z, "INC_SHAPE_ROT_Z" },
  { DEC_SHAPE_ROT_Z, "DEC_SHAPE_ROT_Z" },
  { INC_LIGHT_ROT_X, "INC_LIGHT_ROT_X" },
  { DEC_LIGHT_ROT_X, "DEC_LIGHT_ROT_X" },
  { INC_LIGHT_ROT_Y, "INC_LIGHT_ROT_Y" },
  { DEC_LIGHT_ROT_Y, "DEC_LIGHT_ROT_Y" },
  { INC_LIGHT_ROT_Z, "INC_LIGHT_ROT_Z" },
  { DEC_LIGHT_ROT_Z, "DEC_LIGHT_ROT_Z" },
  { LIGHT_TOGGLE_TYPE, "LIGHT_TOGGLE_TYPE" },
  { START_PAUSE, "START_PAUSE" },
  { NEXT_FRAME, "NEXT_FRAME" },
  { CAMERA_FORWARD, "CAMERA_FORWARD" },
  { CAMERA_BACKWARD, "CAMERA_BACKWARD" },
  { COMMAND_HISTORY, "COMMAND_HISTORY" },
  { SHOW_CONFIG, "SHOW_CONFIG" },
  { EXPORT_CONFIG, "EXPORT_CONFIG" },
};

std::unordered_map<char, operations> keymap = {
  { 'r', RESET_TO_DEFAULT  },
  { '0', ZERO_ROTATION     },
  { 'x', INC_SHAPE_ROT_X   },
  { 'X', DEC_SHAPE_ROT_X   },
  { 'y', INC_SHAPE_ROT_Y   },
  { 'Y', DEC_SHAPE_ROT_Y   },
  { 'z', INC_SHAPE_ROT_Z   },
  { 'Z', DEC_SHAPE_ROT_Z   },
  { 'w', INC_LIGHT_ROT_X   },
  { 's', DEC_LIGHT_ROT_X   },
  { 'a', INC_LIGHT_ROT_Y   },
  { 'd', DEC_LIGHT_ROT_Y   },
  { 'q', INC_LIGHT_ROT_Z   },
  { 'e', DEC_LIGHT_ROT_Z   },
  { 'p', LIGHT_TOGGLE_TYPE },
  { ' ', START_PAUSE       },
  { 'f', NEXT_FRAME        },
  { 'j', CAMERA_FORWARD    },
  { 'k', CAMERA_BACKWARD   },
  { 'h', COMMAND_HISTORY   },
  { 'C', SHOW_CONFIG       },
  { 'E', EXPORT_CONFIG     },
};

void invalidate_computed_frames(int keep) {
  using namespace session;
  LOCK(idx_mtx, hist_mtx);
  int lead = compute_idx - output_idx;
  if (lead < keep) return;
  compute_idx = output_idx + keep;
  buffer_cnt = compute_idx - output_idx;
  retrieve = true;
}

void handle_user_input(char key) {
  using namespace parameter;
  using namespace session;
  switch (keymap[key]) {
    case RESET_TO_DEFAULT: {
      {
        COND_LOCK(is_interactive, params_mtx);
        mutable_params = mutable_params_default;
        setup_camera_movement(mutable_params.camera);
      }
      invalidate_computed_frames(immutable_params.animation.fallback_keep);
      break;
    }
    case ZERO_ROTATION: {
      {
        COND_LOCK(is_interactive, params_mtx);
        mutable_params.shape.rps = { 0, 0, 0 };
      }
      invalidate_computed_frames(immutable_params.animation.fallback_keep);
      break;
    }
    case INC_SHAPE_ROT_X: {
      {
        COND_LOCK(is_interactive, params_mtx);
        mutable_params.shape.rps[X] += mutable_params.shape.delta;
      }
      invalidate_computed_frames(immutable_params.animation.fallback_keep);
      break;
    }
    case DEC_SHAPE_ROT_X: {
      {
        COND_LOCK(is_interactive, params_mtx);
        mutable_params.shape.rps[X] -= mutable_params.shape.delta;
      }
      invalidate_computed_frames(immutable_params.animation.fallback_keep);
      break;
    }
    case INC_SHAPE_ROT_Y: {
      {
        COND_LOCK(is_interactive, params_mtx);
        mutable_params.shape.rps[Y] += mutable_params.shape.delta;
      }
      invalidate_computed_frames(immutable_params.animation.fallback_keep);
      break;
    }
    case DEC_SHAPE_ROT_Y: {
      {
        COND_LOCK(is_interactive, params_mtx);
        mutable_params.shape.rps[Y] -= mutable_params.shape.delta;
      }
      invalidate_computed_frames(immutable_params.animation.fallback_keep);
      break;
    }
    case INC_SHAPE_ROT_Z: {
      {
        COND_LOCK(is_interactive, params_mtx);
        mutable_params.shape.rps[Z] += mutable_params.shape.delta;
      }
      invalidate_computed_frames(immutable_params.animation.fallback_keep);
      break;
    }
    case DEC_SHAPE_ROT_Z: {
      {
        COND_LOCK(is_interactive, params_mtx);
        mutable_params.shape.rps[Z] -= mutable_params.shape.delta;
      }
      invalidate_computed_frames(immutable_params.animation.fallback_keep);
      break;
    }
    case START_PAUSE: {
      if (advance == 0) advance = -1;
      else advance = 0;
      break;
    }
    case NEXT_FRAME: {
      if (advance >= 0) {
        ++advance;
      }
      break;
    }
    case LIGHT_TOGGLE_TYPE: {
      {
        COND_LOCK(is_interactive, params_mtx);
        mutable_params.light.type = (mutable_params.light.type == PARALLEL) ? POINT : PARALLEL;
      }
      invalidate_computed_frames(immutable_params.animation.fallback_keep);
      break;
    }
    case INC_LIGHT_ROT_X: {
      {
        COND_LOCK(is_interactive, params_mtx);
        light_params_t& light = mutable_params.light;
        if (light.type == PARALLEL) light.parallel = geometry::rotate(light.parallel, light.rpp, geometry::X_AXIS);
        else light.point = geometry::rotate(light.point, light.rpp, geometry::X_AXIS);
      }
      invalidate_computed_frames(immutable_params.animation.fallback_keep);
      break;
    }
    case DEC_LIGHT_ROT_X: {
      {
        COND_LOCK(is_interactive, params_mtx);
        light_params_t& light = mutable_params.light;
        if (light.type == PARALLEL) light.parallel = geometry::rotate(light.parallel, -light.rpp, geometry::X_AXIS);
        else light.point = geometry::rotate(light.point, -light.rpp, geometry::X_AXIS);
      }
      invalidate_computed_frames(immutable_params.animation.fallback_keep);
      break;
    }
    case INC_LIGHT_ROT_Y: {
      {
        COND_LOCK(is_interactive, params_mtx);
        light_params_t& light = mutable_params.light;
        if (light.type == PARALLEL) light.parallel = geometry::rotate(light.parallel, light.rpp, geometry::Y_AXIS);
        else light.point = geometry::rotate(light.point, light.rpp, geometry::Y_AXIS);
      }
      invalidate_computed_frames(immutable_params.animation.fallback_keep);
      break;
    }
    case DEC_LIGHT_ROT_Y: {
      {
        COND_LOCK(is_interactive, params_mtx);
        light_params_t& light = mutable_params.light;
        if (light.type == PARALLEL) light.parallel = geometry::rotate(light.parallel, -light.rpp, geometry::Y_AXIS);
        else light.point = geometry::rotate(light.point, -light.rpp, geometry::Y_AXIS);
      }
      invalidate_computed_frames(immutable_params.animation.fallback_keep);
      break;
    }
    case INC_LIGHT_ROT_Z: {
      {
        COND_LOCK(is_interactive, params_mtx);
        light_params_t& light = mutable_params.light;
        if (light.type == PARALLEL) light.parallel = geometry::rotate(light.parallel, light.rpp, geometry::Z_AXIS);
        else light.point = geometry::rotate(light.point, light.rpp, geometry::Z_AXIS);
      }
      invalidate_computed_frames(immutable_params.animation.fallback_keep);
      break;
    }
    case DEC_LIGHT_ROT_Z: {
      {
        COND_LOCK(is_interactive, params_mtx);
        light_params_t& light = mutable_params.light;
        if (light.type == PARALLEL) light.parallel = geometry::rotate(light.parallel, -light.rpp, geometry::Z_AXIS);
        else light.point = geometry::rotate(light.point, -light.rpp, geometry::Z_AXIS);
      }
      invalidate_computed_frames(immutable_params.animation.fallback_keep);
      break;
    }
    case CAMERA_FORWARD: {
      {
        COND_LOCK(is_interactive, params_mtx);
        auto& cam = mutable_params.camera;
        ++cam.idx;
        if (cam.idx == cam.steps) cam.idx = cam.steps - 1;
      }
      invalidate_computed_frames(immutable_params.animation.fallback_keep);
      break;
    }
    case CAMERA_BACKWARD: {
      {
        COND_LOCK(is_interactive, params_mtx);
        auto& cam = mutable_params.camera;
        --cam.idx;
        if (cam.idx == -1) cam.idx = 0;
      }
      invalidate_computed_frames(immutable_params.animation.fallback_keep);
      break;
    }
    case SHOW_CONFIG: {
      break;
    }
    case COMMAND_HISTORY: {
      break;
    }
    case EXPORT_CONFIG: {
      // clear screen, ask for filename, and write to file
      break;
    }
  }
}

}
