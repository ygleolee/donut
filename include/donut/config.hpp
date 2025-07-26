#pragma once

#include "donut/control.hpp"
#include "donut/parameter.hpp"

#include <toml++/toml.hpp>

#include <unordered_map>

namespace donut::config {

using namespace parameter;

void parse_light_config(toml::table* light_config, light_params_t& light, vst& errors);
void parse_camera_config(toml::table* camera_config, camera_params_t& camera, vst& errors);
void parse_shape_config(toml::table* shape_config, shape_params_t& shape, vst& errors);

void parse_display_config(toml::table* display_config, display_params_t& display, vst& errors);
void parse_animation_config(toml::table* animation_config, animation_params_t& animation, vst& errors);
void parse_control_config(toml::table* control_config, control_params_t& control, vst& errors);

void parse_keymap_config(toml::table* keymap_config, std::unordered_map<char, control::operations_t>& keymap, vst& errors);

vst parse_config(mutable_params_t& mutable_params, immutable_params_t& immutable_params, std::unordered_map<char, control::operations_t>& keymap, const std::string& filename);

void serialize_config(const mutable_params_t& mutable_params, const immutable_params_t& immutable_params, const std::unordered_map<char, control::operations_t>& keymap, const std::string& filename);

}
