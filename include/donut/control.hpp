# pragma once

#include <unordered_map>

#define RESET                1
#define ZERO_ROTATION        2
#define INCREASE_SHAPE_ROT_X 3
#define DECREASE_SHAPE_ROT_X 4
#define INCREASE_SHAPE_ROT_Y 5
#define DECREASE_SHAPE_ROT_Y 6
#define INCREASE_SHAPE_ROT_Z 7
#define DECREASE_SHAPE_ROT_Z 8
#define INCREASE_LIGHT_ROT_X 9
#define DECREASE_LIGHT_ROT_X 10
#define INCREASE_LIGHT_ROT_Y 11
#define DECREASE_LIGHT_ROT_Y 12
#define INCREASE_LIGHT_ROT_Z 13
#define DECREASE_LIGHT_ROT_Z 14
#define LIGHT_TOGGLE_TYPE    15
#define START_PAUSE          16
#define NEXT_FRAME           17
#define CAMERA_FORWARD       18
#define CAMERA_BACKWARD      19
#define SHOW_CONFIG          20
#define COMMAND_HISTORY      21

namespace donut::control {

  extern std::unordered_map<char, int> key_mappings;

  void setup_default_keymap(std::unordered_map<char, int>& keymap);

  void invalidate_computed_frames(int keep); // move compute_idx to n away from output_idx for responsiveness
  void handle_user_input(int read, char buf[3]);

}
