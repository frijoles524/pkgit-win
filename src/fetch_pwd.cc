#include "fetch_pwd.hh"
#include "vars.hh"

void fetch_pwd(Pkg pkg) {
  std::filesystem::copy(std::filesystem::current_path(), pkg.src);
}
