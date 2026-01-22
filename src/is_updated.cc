#include "cmd_out.cc"

bool is_updated(std::string src) {
  if (cmd_out("git pull") == "Already up to date.") { return true; }
  return false;
}
