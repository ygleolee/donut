#include "donut/config.hpp"
#include "donut/control.hpp"

#include <toml++/toml.hpp>

#include <iostream>

std::pair<vec, bool> parse_vec(const toml::array& arr) {
  std::pair<vec, bool> err_res = { {}, false };
  if (arr.size() != 3) {
    return err_res;
  }
  vec res;
  for (int i=0; i<3; ++i) {
    if (auto val = arr[i].value<double>()) {
      res[i] = *val;
    }
    else {
      return err_res;
    }
  }
  return { res, true };
}

toml::array to_array(const vec& v) {
  toml::array arr;
  arr.push_back(v[0]);
  arr.push_back(v[1]);
  arr.push_back(v[2]);
  return arr;
}

namespace donut::config {

void parse_light_config(toml::table* light_config, parameter::light_params_t& light, vst& errors){
  if (auto type_ptr = light_config->get("type")) {
    if (auto type = type_ptr->value<std::string>()) {
      if (*type == "PARALLEL") {
        light.type = PARALLEL;
      }
      else if (*type == "POINT") {
        light.type = POINT;
      }
      else {
        errors.push_back("Invalid light.type: must be \"PARALLEL\" or \"POINT\"");
      }
    }
  }

  if (auto parallel_ptr = light_config->get("parallel")) {
    auto res = parse_vec(*(parallel_ptr->as_array()));
    if (res.second) {
      light.parallel = res.first;
    }
    else {
      errors.push_back("Invalid light.parallel: expected an array of 3 floating point numbers");
    }
  }


  if (auto point_ptr = light_config->get("point")) {
    auto res = parse_vec(*(point_ptr->as_array()));
    if (res.second) {
      light.point = res.first;
    }
    else {
      errors.push_back("Invalid light.point: expected an array of 3 floating point numbers");
    }
  }

  if (auto rps_ptr = light_config->get("rps")) {
    auto res = parse_vec(*(rps_ptr->as_array()));
    if (res.second) {
      light.rps = res.first;
    }
    else {
      errors.push_back("Invalid light.rps: expected an array of 3 floating point numbers");
    }
  }

  if (auto rpp_ptr = light_config->get("rpp")) {
    if (auto rpp = rpp_ptr->value<double>()) {
      light.rpp = *rpp;
    }
    else {
      errors.push_back("Invalid light.rpp: expected a floating point number");
    }
  }
}

void parse_camera_config(toml::table* camera_config, parameter::camera_params_t& camera, vst& errors) {
  if (auto min_ptr = camera_config->get("min")) {
    if (auto min = min_ptr->value<double>()) {
      camera.min = *min;
    }
    else {
      errors.push_back("Invalid camera.min: expected a floating point number");
    }
  }
  if (auto max_ptr = camera_config->get("max")) {
    if (auto max = max_ptr->value<double>()) {
      camera.max = *max;
    }
    else {
      errors.push_back("Invalid camera.max: expected a floating point number");
    }
  }
  if (auto steps_ptr = camera_config->get("steps")) {
    if (auto steps = steps_ptr->value<unsigned int>()) {
      camera.steps = *steps;
    }
    else {
      errors.push_back("Invalid camera.steps: expected a positive integer");
    }
  }
}

void parse_shape_config(toml::table* shape_config, parameter::shape_params_t& shape, vst& errors) {
  if (auto rps_ptr = shape_config->get("rps")) {
    auto res = parse_vec(*(rps_ptr->as_array()));
    if (res.second) {
      shape.rps = res.first;
    }
    else {
      errors.push_back("Invalid shape.rps: expected an array of 3 floating point numbers");
    }
  }
  if (auto delta_ptr = shape_config->get("delta")) {
    if (auto delta = delta_ptr->value<double>()) {
      shape.delta = *delta;
    }
    else {
      errors.push_back("Invalid shape.delta: expected a floating point number");
    }
  }
}

void parse_display_config(toml::table* display_config, parameter::display_params_t& display, vst& errors) {
  if (auto grayscale_ptr = display_config->get("grayscale")) {
    if (auto grayscale = grayscale_ptr->value<std::string>()) {
      display.grayscale = *grayscale;
    }
    else {
      errors.push_back("Invalid display.grayscale: expected a string with ascii characters");
    }
  }
  if (auto range_ptr = display_config->get("range")) {
    if (auto range = range_ptr->value<double>()) {
      display.range = *range;
    }
    else {
      errors.push_back("Invalid display.range: expected a floating point number");
    }
  }
  if (auto char_ratio_ptr = display_config->get("char_ratio")) {
    if (auto char_ratio = char_ratio_ptr->value<double>()) {
      display.char_ratio = *char_ratio;
    }
    else {
      errors.push_back("Invalid display.char_ratio: expected a floating point number");
    }
  }
  if (auto fps_ptr = display_config->get("fps")) {
    if (auto fps = fps_ptr->value<unsigned int>()) {
      display.fps = *fps;
    }
    else {
      errors.push_back("Invalid display.fps: expected a positive integer");
    }
  }
}

void parse_keymap_config(toml::table* keymap_config, std::unordered_map<char, control::operations>& keymap, vst& errors) {
  using namespace control;
  for (const auto& [cmd, ch] : *keymap_config) {
    std::string op = std::string(cmd.str());
    auto keys = ch.value<std::string>();
    if (keys && keys->size() == 1) {
      if (auto it = operations_map.find(op); it != operations_map.end()) {
        keymap[(*keys)[0]] = it->second;
      }
      else {
        errors.push_back("Invalid operation " + op);
      }
    }
    else {
      errors.push_back("Invalid keymap for " + op + ": mapped key must be a string with a single character");
    }
  }
}

vst parse_config(mutable_params_t& mutable_params, immutable_params_t& immutable_params, std::unordered_map<char, control::operations>& keymap, const std::string& filename) {
  vst errors;
  try {
    auto config = toml::parse_file(filename);

    // mutable params
    if (auto light_config = config["light"].as_table()) {
      parse_light_config(light_config, mutable_params.light, errors);
    }
    if (auto camera_config = config["camera"].as_table()) {
      parse_camera_config(camera_config, mutable_params.camera, errors);
    }
    if (auto shape_config = config["shape"].as_table()) {
      parse_shape_config(shape_config, mutable_params.shape, errors);
    }

    // immutable params
    if (auto display_config = config["display"].as_table()) {
      parse_display_config(display_config, immutable_params.display, errors);
    }
    if (auto animation_config = config["animation"].as_table()) {
      parse_animation_config(animation_config, immutable_params.animation, errors);
    }
    if (auto control_config = config["control"].as_table()) {
      parse_control_config(control_config, immutable_params.control, errors);
    }

    // keymap
    if (auto keymap_config = config["keymap"].as_table()) {
      parse_keymap_config(keymap_config, keymap, errors);
    }
  } catch (const toml::parse_error& err) {
    std::cerr << "Parse error: " << err.description() << "\n  at " << err.source().begin << "\n";
    return errors;
  }

  return errors;
}

void serialize_config(const mutable_params_t& mutable_params, const immutable_params_t& immutable_params, const std::unordered_map<char, control::operations>& keymap, const std::string& filename) {
  toml::table config;

  // [light]
  toml::table light_tbl;
  light_tbl.insert("type", mutable_params.light.type == PARALLEL ? "PARALLEL" : "POINT");
  light_tbl.insert("parallel", to_array(mutable_params.light.parallel));
  light_tbl.insert("point", to_array(mutable_params.light.point));
  light_tbl.insert("rps", to_array(mutable_params.light.rps));
  light_tbl.insert("rpp", mutable_params.light.rpp);
  config.insert("light", light_tbl);

  // [camera]
  toml::table camera_tbl;
  camera_tbl.insert("min", mutable_params.camera.min);
  camera_tbl.insert("max", mutable_params.camera.max);
  camera_tbl.insert("steps", mutable_params.camera.steps);
  config.insert("camera", camera_tbl);

  // [shape]
  toml::table shape_tbl;
  shape_tbl.insert("rps", to_array(mutable_params.shape.rps));
  shape_tbl.insert("delta", mutable_params.shape.delta);
  config.insert("shape", shape_tbl);

  // [display]
  toml::table display_tbl;
  display_tbl.insert("grayscale", immutable_params.display.grayscale);
  display_tbl.insert("range", immutable_params.display.range);
  display_tbl.insert("char_ratio", immutable_params.display.char_ratio);
  display_tbl.insert("fps", immutable_params.display.fps);
  config.insert("display", display_tbl);

  // [animation]
  toml::table animation_tbl;
  animation_tbl.insert("buffer_size", immutable_params.animation.buffer_size);
  animation_tbl.insert("fallback_keep", immutable_params.animation.fallback_keep);
  config.insert("animation", animation_tbl);
 
  // [control]
  toml::table control_tbl;
  control_tbl.insert("debounce", immutable_params.control.debounce);
  config.insert("contorl", control_tbl);

  // [keymap]
  toml::table keymap_tbl;
  for (const auto& [key, op] : keymap) {
    auto it = control::reverse_operations_map.find(op);
    if (it != control::reverse_operations_map.end()) {
      std::string command = it->second;
      std::string key_str(1, key); // single-char string
      keymap_tbl.insert(command, key_str);
    }
  }
  config.insert("keymap", keymap_tbl);

  // Write to file
  std::ofstream out(filename);
  out << std::fixed << std::setprecision(2);
  out << config;
}

}
