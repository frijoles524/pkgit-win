#include <filesystem>
#include <iostream>

#include "build.hh"
#include "lua_build.hh"
#include "vars.hh"

void build(Pkg pkg) {
  if (pkg.src != std::filesystem::current_path().string()) {
    std::filesystem::current_path(pkg.src);
  }
  if (lua_build(pkg.name.c_str(), pkg.target, pkg.src.c_str())) {
    return;
  } else {
    std::cout << print_error << "no usable build system was found\n";
  }
}