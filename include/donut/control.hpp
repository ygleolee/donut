# pragma once

#include <unordered_map>

namespace donut::control {

enum operations {
  RESET_TO_DEFAULT,
  ZERO_ROTATION,
  INC_SHAPE_ROT_X,
  DEC_SHAPE_ROT_X,
  INC_SHAPE_ROT_Y,
  DEC_SHAPE_ROT_Y,
  INC_SHAPE_ROT_Z,
  DEC_SHAPE_ROT_Z,
  INC_LIGHT_ROT_X,
  DEC_LIGHT_ROT_X,
  INC_LIGHT_ROT_Y,
  DEC_LIGHT_ROT_Y,
  INC_LIGHT_ROT_Z,
  DEC_LIGHT_ROT_Z,
  LIGHT_TOGGLE_TYPE,
  START_PAUSE,
  NEXT_FRAME,
  CAMERA_FORWARD,
  CAMERA_BACKWARD,
  SHOW_CONFIG,
  COMMAND_HISTORY,
  EXPORT_CONFIG,
};

extern const std::unordered_map<std::string, operations> operations_map;
extern const std::unordered_map<operations, std::string> reverse_operations_map;
extern std::unordered_map<char, operations> key_mappings;

void setup_default_keymap(std::unordered_map<char, operations>& keymap);

void invalidate_computed_frames(int keep); // move compute_idx to n away from output_idx for responsiveness
void handle_user_input(int read, char buf[3]);

}
