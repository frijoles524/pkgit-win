#include <filesystem>

#include "is_updated.hh"
#include "cmd_out.hh"

bool is_updated(std::string src) {
  if (src != std::filesystem::current_path().string()) {
		if (!std::filesystem::exists(src)) { return false; }
    std::filesystem::current_path(src);
  }
  if (cmd_out("git pull") == "Already up to date.") { return true; }
  return false;
}
