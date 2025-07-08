# pragma once

namespace donut::control {

  void invalidate_computed_frames(int keep); // move compute_idx to n away from output_idx for responsiveness
  void handle_user_input(int read, char buf[3]);

}
