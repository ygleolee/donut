#pragma once

#include "donut/control.hpp"
#include "donut/parameter.hpp"

#include <toml++/toml.hpp>

#include <unordered_map>

namespace donut::config {

void parse_light_config(toml::table* light_config, parameter::params_t& params, vst& errors);
void parse_camera_config(toml::table* camera_config, parameter::params_t& params, vst& errors);
void parse_shape_config(toml::table* shape_config, parameter::params_t& params, vst& errors);
void parse_display_config(toml::table* display_config, parameter::params_t& params, vst& errors);
void parse_keymap_config(toml::table* keymap_config, std::unordered_map<char, control::operations>& keymap, vst& errors);
vst parse_config(parameter::params_t& params, std::unordered_map<char, control::operations>& keymap, const std::string& filename);
void serialize_config(const parameter::params_t& params, const std::unordered_map<char, control::operations>& keymap, const std::string& filename);

}
