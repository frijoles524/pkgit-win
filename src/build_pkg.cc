#include <functional>

#include "build_map.cc"

void build_pkg(std::filesystem::path build_dir) {
  if (build_dir != fs::current_path().string()) {
    fs::current_path(build_dir);
  }
  for (auto const &dir_entry :
       fs::directory_iterator(fs::current_path().string())) {
    for (auto build : builds) {
      if (dir_entry.path().filename() == build.first) {
        build.second();
      }
    }
  }
}
