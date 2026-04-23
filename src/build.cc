#include <filesystem>
#include <iostream>

#include "build.hh"
#include "lua_build.hh"
#include "vars.hh"

void build(std::filesystem::path build_dir) {
  if (build_dir != std::filesystem::current_path().string()) {
    std::filesystem::current_path(build_dir);
  }

  bool bldit_found = false;
  bool build_found = false;

  for (auto const &dir_entry : std::filesystem::directory_iterator(std::filesystem::current_path().string())) {
    if (dir_entry.path().filename() == "bldit") {
      bldit_found = true;
      break;
    }
  }

  if (bldit_found) {
    system("./bldit");
  } else if (lua_build(build_dir.c_str())) {
    return;
  } else {
    std::cout << print_error << "no usable build system was found\n";
  }
}