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

void parse_light_config(toml::table* light_config, parameter::params_t& params, vst& errors){
  if (auto type_ptr = light_config->get("type")) {
    if (auto type = type_ptr->value<std::string>()) {
      if (*type == "PARALLEL") {
        params.light.type = PARALLEL;
      }
      else if (*type == "POINT") {
        params.light.type = POINT;
      }
      else {
        errors.push_back("Invalid light.type: must be \"PARALLEL\" or \"POINT\"");
      }
    }
  }

  if (auto parallel_ptr = light_config->get("parallel")) {
    auto res = parse_vec(*(parallel_ptr->as_array()));
    if (res.second) {
      params.light.parallel = res.first;
    }
    else {
      errors.push_back("Invalid light.parallel: expected an array of 3 floating point numbers");
    }
  }


  if (auto point_ptr = light_config->get("point")) {
    auto res = parse_vec(*(point_ptr->as_array()));
    if (res.second) {
      params.light.point = res.first;
    }
    else {
      errors.push_back("Invalid light.point: expected an array of 3 floating point numbers");
    }
  }

  if (auto rps_ptr = light_config->get("rps")) {
    auto res = parse_vec(*(rps_ptr->as_array()));
    if (res.second) {
      params.light.rps = res.first;
    }
    else {
      errors.push_back("Invalid light.rps: expected an array of 3 floating point numbers");
    }
  }

  if (auto rpp_ptr = light_config->get("rpp")) {
    if (auto rpp = rpp_ptr->value<double>()) {
      params.light.rpp = *rpp;
    }
    else {
      errors.push_back("Invalid light.rpp: expected a floating point number");
    }
  }
}
void parse_camera_config(toml::table* camera_config, parameter::params_t& params, vst& errors) {
  if (auto min_ptr = camera_config->get("min")) {
    if (auto min = min_ptr->value<double>()) {
      params.camera.min = *min;
    }
    else {
      errors.push_back("Invalid camera.min: expected a floating point number");
    }
  }
  if (auto max_ptr = camera_config->get("max")) {
    if (auto max = max_ptr->value<double>()) {
      params.camera.max = *max;
    }
    else {
      errors.push_back("Invalid camera.max: expected a floating point number");
    }
  }
  if (auto steps_ptr = camera_config->get("steps")) {
    if (auto steps = steps_ptr->value<unsigned int>()) {
      params.camera.steps = *steps;
    }
    else {
      errors.push_back("Invalid camera.steps: expected a positive integer");
    }
  }
}

void parse_shape_config(toml::table* shape_config, parameter::params_t& params, vst& errors) {
  if (auto rps_ptr = shape_config->get("rps")) {
    auto res = parse_vec(*(rps_ptr->as_array()));
    if (res.second) {
      params.shape.rps = res.first;
    }
    else {
      errors.push_back("Invalid shape.rps: expected an array of 3 floating point numbers");
    }
  }
  if (auto delta_ptr = shape_config->get("delta")) {
    if (auto delta = delta_ptr->value<double>()) {
      params.shape.delta = *delta;
    }
    else {
      errors.push_back("Invalid shape.delta: expected a floating point number");
    }
  }
}

void parse_display_config(toml::table* display_config, parameter::params_t& params, vst& errors) {
  if (auto grayscale_ptr = display_config->get("grayscale")) {
    if (auto grayscale = grayscale_ptr->value<std::string>()) {
      params.display.grayscale = *grayscale;
    }
    else {
      errors.push_back("Invalid display.grayscale: expected a string with ascii characters");
    }
  }
  if (auto range_ptr = display_config->get("range")) {
    if (auto range = range_ptr->value<double>()) {
      params.display.range = *range;
    }
    else {
      errors.push_back("Invalid display.range: expected a floating point number");
    }
  }
  if (auto char_ratio_ptr = display_config->get("char_ratio")) {
    if (auto char_ratio = char_ratio_ptr->value<double>()) {
      params.display.char_ratio = *char_ratio;
    }
    else {
      errors.push_back("Invalid display.char_ratio: expected a floating point number");
    }
  }
  if (auto fps_ptr = display_config->get("fps")) {
    if (auto fps = fps_ptr->value<unsigned int>()) {
      params.display.fps = *fps;
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

vst parse_config(parameter::params_t& params, std::unordered_map<char, control::operations>& keymap, const std::string& filename) {
  vst errors;
  try {
    auto config = toml::parse_file(filename);
    if (auto light_config = config["light"].as_table()) {
      parse_light_config(light_config, params, errors);
    }
    if (auto camera_config = config["camera"].as_table()) {
      parse_camera_config(camera_config, params, errors);
    }
    if (auto shape_config = config["shape"].as_table()) {
      parse_shape_config(shape_config, params, errors);
    }
    if (auto display_config = config["display"].as_table()) {
      parse_display_config(display_config, params, errors);
    }
    if (auto keymap_config = config["keymap"].as_table()) {
      parse_keymap_config(keymap_config, keymap, errors);
    }
  } catch (const toml::parse_error& err) {
    std::cerr << "Parse error: " << err.description() << "\n  at " << err.source().begin << "\n";
    return errors;
  }

  return errors;
}

void serialize_config(const parameter::params_t& params, const std::unordered_map<char, control::operations>& keymap, const std::string& filename) {
  toml::table config;

  // [light]
  toml::table light_tbl;
  light_tbl.insert("type", params.light.type == PARALLEL ? "PARALLEL" : "POINT");
  light_tbl.insert("parallel", to_array(params.light.parallel));
  light_tbl.insert("point", to_array(params.light.point));
  light_tbl.insert("rps", to_array(params.light.rps));
  light_tbl.insert("rpp", params.light.rpp);
  config.insert("light", light_tbl);

  // [camera]
  toml::table camera_tbl;
  camera_tbl.insert("min", params.camera.min);
  camera_tbl.insert("max", params.camera.max);
  camera_tbl.insert("steps", params.camera.steps);
  config.insert("camera", camera_tbl);

  // [shape]
  toml::table shape_tbl;
  shape_tbl.insert("rps", to_array(params.shape.rps));
  shape_tbl.insert("delta", params.shape.delta);
  config.insert("shape", shape_tbl);

  // [display]
  toml::table display_tbl;
  display_tbl.insert("grayscale", params.display.grayscale);
  display_tbl.insert("range", params.display.range);
  display_tbl.insert("char_ratio", params.display.char_ratio);
  display_tbl.insert("fps", params.display.fps);
  config.insert("display", display_tbl);

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
