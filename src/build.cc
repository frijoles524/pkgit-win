#include <filesystem>

#include "lua_build.cc"

void build(std::filesystem::path build_dir) {
  if (build_dir != std::filesystem::current_path().string()) {
    std::filesystem::current_path(build_dir);
  }
  for (auto const &dir_entry : std::filesystem::directory_iterator(fs::current_path().string())) {
    if (dir_entry.path().filename() == "bldit") { system("./bldit"); }
    else { lua_build(build_dir.c_str()); }
  }
}
