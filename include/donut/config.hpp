#pragma once

#include "donut/parameter.hpp"

#include <unordered_map>

namespace donut::config {

std::vector<std::string> parse_config(struct parameter::params& params, std::unordered_map<char, int>& keymap, std::string filename);

}
