#include <filesystem>

#include "setup_dirs.hh"
#include "vars.hh"

void setup_dirs() {
  for(unsigned int a = 0; a < sizeof(all_dirs)/sizeof(all_dirs[0]); a++) {
    std::filesystem::create_directories(all_dirs[a]);
  }
}
