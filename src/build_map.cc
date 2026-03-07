#include <functional>

#include "build_systems.cc"

const std::map<std::string, std::function<void()>> builds = {
  {"bldit",           bldit_build},
  {"compile.sh",      compilesh_build},
  {"build.sh",        buildsh_build},
  {"autogen.sh",      autogen_build},
  {"configure",       autotools_build},
  {"configure.ac",    autotools_build},
  {"Makefile",        make_build},
  {"Makefile.am",     make_build},
  {"CMakeLists.txt",  cmake_build},
  {"meson.build",     meson_build},
  {"build.ninja",     ninja_build},
  {"Cargo.toml",      cargo_build},
  {"go.mod",          go_build},
  {"gradle.build",    gradle_build},
  {"pnpm-lock.yaml",  pnpm_build},
  {"pyproject.toml",  python_build},
  {"build.zig",       zig_build}
};

