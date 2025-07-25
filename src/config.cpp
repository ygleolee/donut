#include "donut/config.hpp"

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

namespace donut::config {

std::vector<std::string> parse_config(struct parameter::params& params, std::unordered_map<char, int>& keymap, std::string& filename) {
  std::vector<std::string> errors;
  try {
    auto config = toml::parse_file(filename);
    if (auto l = config["light"].as_table()) {
      // params.light.type = parse_light_type(l->get("type")->value_or("PARALLEL"));
      if (auto type_ptr = l->get("type")) {
        if (auto type = type_ptr->value<std::string>()) {
          if (*type == "PARALLEL") {
            params.light.type = PARALLEL;
          }
          else if (*type == "POINT") {
            params.light.type = POINT;
          }
          else {
            errors.push_back("Invalid light.type, must be \"PARALLEL\" or \"POINT\"");
          }
        }
      }

      if (auto parallel_ptr = l->get("parallel")) {
        auto res = parse_vec(*(parallel_ptr->as_array()));
        if (res.second) {
          params.light.parallel = res.first;
        }
        else {
          errors.push_back("Invalid light.parallel, expected an array of 3 floating point numbers");
        }
      }


      if (auto point_ptr = l->get("point")) {
        auto res = parse_vec(*(point_ptr->as_array()));
        if (res.second) {
          params.light.point = res.first;
        }
        else {
          errors.push_back("Invalid light.point, expected an array of 3 floating point numbers");
        }
      }

      if (auto rps_ptr = l->get("rps")) {
        auto res = parse_vec(*(rps_ptr->as_array()));
        if (res.second) {
          params.light.rps = res.first;
        }
        else {
          errors.push_back("Invalid light.rps, expected an array of 3 floating point numbers");
        }
      }

      if (auto rpp_ptr = l->get("rpp")) {
        if (auto rpp = rpp_ptr->value<double>()) {
          params.light.rpp = *rpp;
        }
        else {
          errors.push_back("Invalid light.rpp, expected a floating point number");
        }
      }
    }
   
    if (auto c = config["camera"].as_table()) {
      if (auto min_ptr = c->get("min")) {
        if (auto min = min_ptr->value<double>()) {
          params.camera.min = *min;
        }
        else {
          errors.push_back("Invalid camera.min, expected a floating point number");
        }
      }
      if (auto max_ptr = c->get("max")) {
        if (auto max = max_ptr->value<double>()) {
          params.camera.max = *max;
        }
        else {
          errors.push_back("Invalid camera.max, expected a floating point number");
        }
      }
      if (auto steps_ptr = c->get("steps")) {
        if (auto steps = steps_ptr->value<unsigned int>()) {
          params.camera.steps = *steps;
        }
        else {
          errors.push_back("Invalid camera.steps, expected a positive integer");
        }
      }
    }
   
    if (auto s = config["shape"].as_table()) {
      if (auto rps_ptr = s->get("rps")) {
        auto res = parse_vec(*(rps_ptr->as_array()));
        if (res.second) {
          params.shape.rps = res.first;
        }
        else {
          errors.push_back("Invalid shape.rps, expected an array of 3 floating point numbers");
        }
      }
      if (auto delta_ptr = s->get("delta")) {
        if (auto delta = delta_ptr->value<double>()) {
          params.shape.delta = *delta;
        }
        else {
          errors.push_back("Invalid shape.delta, expected a floating point number");
        }
      }
    }
   
    if (auto d = config["display"].as_table()) {
      if (auto grayscale_ptr = d->get("grayscale")) {
        if (auto grayscale = grayscale_ptr->value<std::string>()) {
          params.display.grayscale = *grayscale;
        }
        else {
          errors.push_back("Invalid display.grayscale, expected a string with ascii characters");
        }
      }
      if (auto range_ptr = d->get("range")) {
        if (auto range = range_ptr->value<double>()) {
          params.display.range = *range;
        }
        else {
          errors.push_back("Invalid display.range, expected a floating point number");
        }
      }
      if (auto char_ratio_ptr = d->get("char_ratio")) {
        if (auto char_ratio = char_ratio_ptr->value<double>()) {
          params.display.char_ratio = *char_ratio;
        }
        else {
          errors.push_back("Invalid display.char_ratio, expected a floating point number");
        }
      }
      if (auto fps_ptr = d->get("fps")) {
        if (auto fps = fps_ptr->value<unsigned int>()) {
          params.display.fps = *fps;
        }
        else {
          errors.push_back("Invalid display.fps, expected a positive integer");
        }
      }
    }
  } catch (const toml::parse_error& err) {
    std::cerr << "Parse error: " << err.description() << "\n  at " << err.source().begin << "\n";
    return errors;
  }

  return errors;
}

}
