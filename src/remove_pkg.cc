#include <filesystem>
#include <iostream>
#include <string>
#include <unistd.h>

#include "remove_pkg.hh"
#include "vars.hh"

void remove_pkg(Pkg pkg) {
  if (!std::filesystem::exists(pkg.src)) {
    std::cout << print_pkgit << pkg.name << " is not installed!" << std::endl;
    return;
  }
  for (auto const& dir_entry : std::filesystem::recursive_directory_iterator(pkg.src)) {
    if (dir_entry.path().extension().string().rfind(".so", 0) == 0) {
      std::filesystem::remove(install_directories["lib"]+"/"+dir_entry.path().filename().string());
      //std::cout << print_pkgit << "removed library: " << dir_entry << "\n";

    } else if (!access(dir_entry.path().c_str(), X_OK) && !is_directory(dir_entry.path())) {
      std::filesystem::remove(install_directories["bin"]+"/"+dir_entry.path().filename().string());
      //std::cout << print_pkgit << "removed executable: " << dir_entry << "\n";

    } else if (dir_entry.path().extension() == ".h") {
      std::filesystem::remove(install_directories["include"]+"/"+dir_entry.path().filename().string());
      //std::cout << print_pkgit << "removed include: " << dir_entry << "\n";
    }
  }
  remove_all(pkg.src.parent_path());
  std::cout << print_pkgit << "removed " << pkg.name << "\n";
}
