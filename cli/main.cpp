#include "donut/session.hpp"
#include "donut/shapes.hpp"

#include <CLI/CLI.hpp>

#include <filesystem>
#include <iostream>
#include <string>

std::string try_get_config_path() {
  // check $DONUT_CONFIG_PATH
  const char* env_path = std::getenv("DONUT_CONFIG_PATH");
  if (env_path != nullptr) {
    std::string path = env_path;
    if (path.size() >= 5 && path.substr(path.size() - 5) == ".toml") {
      if (std::filesystem::exists(path)) {
        return path;
      } else {
        std::cerr << "Error: $DONUT_CONFIG_PATH points to non-existent file: " << path << "\n";
        exit(1);
      }
    } else {
      std::cerr << "Error: $DONUT_CONFIG_PATH must be a .toml file: " << path << "\n";
      exit(1);
    }
  }

  // check ~/.config/donut/config.toml
  const char* home = std::getenv("HOME");
  if (home != nullptr) {
    std::filesystem::path fallback_path = std::filesystem::path(home) / ".config" / "donut" / "config.toml";
    if (std::filesystem::exists(fallback_path)) {
      return fallback_path.string();
    }
  }

  return "";
}

int main(int argc, char** argv) {
  CLI::App app{"donut"};

  // global options
  bool interactive = false;
  std::string config_path = "";
  app.add_flag("-i,--interactive", interactive, "interactive mode");
  app.add_option("-c,--config", config_path, "configuration file")->type_name("<filename>");

  // shapes as subcommands

  // donut
  auto donut_cmd = app.add_subcommand("donut");
  double donut_r1 = 60.0, donut_r2 = 30.0;
  donut_cmd->add_option("--r1", donut_r1, "major radius");
  donut_cmd->add_option("--r2", donut_r2, "minor radius");

  // mister donut
  auto mister_donut_cmd = app.add_subcommand("mister_donut");
  double mister_donut_r1 = 60.0, mister_donut_r2 = 24.0;
  int mister_donut_n = 8;
  mister_donut_cmd->add_option("--r1", mister_donut_r1, "major radius");
  mister_donut_cmd->add_option("--r2", mister_donut_r2, "spheres radius");
  mister_donut_cmd->add_option("--n", mister_donut_n, "number of spheres");

  // cuboid
  auto cuboid_cmd = app.add_subcommand("cuboid");
  double cuboid_w = 60.0, cuboid_h = 90.0, cuboid_l = 70.0;
  cuboid_cmd->add_option("--w", cuboid_w, "width");
  cuboid_cmd->add_option("--h", cuboid_h, "height");
  cuboid_cmd->add_option("--l", cuboid_l, "length");

  // ellipsoid
  auto ellipsoid_cmd = app.add_subcommand("ellipsoid");
  double ellipsoid_w = 60.0, ellipsoid_h = 60.0, ellipsoid_l = 60.0;
  ellipsoid_cmd->add_option("--w", ellipsoid_w, "width");
  ellipsoid_cmd->add_option("--h", ellipsoid_h, "height");
  ellipsoid_cmd->add_option("--l", ellipsoid_l, "length");

  CLI11_PARSE(app, argc, argv);

  shape_data data;
  if (*mister_donut_cmd) {
    data = donut::shapes::mister_donut(mister_donut_r1, mister_donut_r2, mister_donut_n);
  }
  else if (*cuboid_cmd) {
    data = donut::shapes::cuboid(cuboid_w, cuboid_h, cuboid_l);
  }
  else if (*ellipsoid_cmd) {
    data = donut::shapes::ellipsoid(ellipsoid_w, ellipsoid_h, ellipsoid_l);
  }
  else { // default to donut
    data = donut::shapes::donut(donut_r1, donut_r2);
  }

  if (config_path == "") {
    config_path = try_get_config_path();
  }

  donut::session::entry(data, interactive, config_path);

  return 0;
}
