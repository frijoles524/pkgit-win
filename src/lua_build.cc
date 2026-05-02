#include "lua_build.hh"
#include "lua_state.hh"
#include "vars.hh"
#include <filesystem>
#include <iostream>
#include <unordered_map>

bool lua_build(const char *repository, const char *target, const char *path) {
  std::cout << print_pkgit << "attempting to use build function specified in 'repositories." << repository << "'..." << std::endl;
  if (repo_build(repository)) {
    return true;
  }

  std::cout << print_pkgit << "attempting to use build function specified in 'bldit.lua'..." << std::endl;
  if (bldit(target)) {
    return true;
  }

  std::cout << print_pkgit << "attempting to use build functions specified in 'build_systems'..." << std::endl;
  if (config_build(path)) {
    return true;
  }

  return false;
}