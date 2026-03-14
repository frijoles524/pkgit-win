#include "vars.cc"

void fetch_pwd(Pkg pkg) {
  std::filesystem::copy(std::filesystem::current_path(), pkg.src);
}
