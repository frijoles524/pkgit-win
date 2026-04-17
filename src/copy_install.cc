#include <filesystem>
#include <iostream>
#include <string>
#include <unistd.h>

#include "copy_install.hh"
#include "vars.hh"

void copy_install(std::filesystem::path build_dir) {
  for (auto const& dir_entry : std::filesystem::recursive_directory_iterator(build_dir)) {
    if (dir_entry.path().extension().string().rfind(".so", 0) == 0) {
      if (!std::filesystem::exists(install_directories["lib"]+"/"+dir_entry.path().filename().string())) {
        copy(dir_entry, install_directories["lib"]+"/"+dir_entry.path().filename().string());
        if (is_verbose) { std::cout << print_pkgit << "copied library: " << dir_entry << "\n"; }
      } else { /*std::cout << print_pkgit << "library already exists: " << dir_entry << "\n";*/ }

    } else if (!access(dir_entry.path().c_str(), X_OK) && !is_directory(dir_entry.path())) {
      if (!std::filesystem::exists(install_directories["bin"]+"/"+dir_entry.path().filename().string()) &&
    			dir_entry.path().extension() != ".sample" &&
          dir_entry.path().filename().string() != "bldit" &&
          dir_entry.path().filename().string() != "build.sh" &&
          dir_entry.path().filename().string() != "compile.sh") {
        copy(dir_entry, install_directories["bin"]+"/"+dir_entry.path().filename().string());
        if (is_verbose) { std::cout << print_pkgit << "copied executable: " << dir_entry << "\n"; }
      } else { /*std::cout << print_pkgit << "executable already exists: " << dir_entry << "\n";*/ }

    } else if (dir_entry.path().extension() == ".h") {
      if (!std::filesystem::exists(install_directories["include"]+"/"+dir_entry.path().filename().string())) {
        copy(dir_entry, install_directories["include"]+"/"+dir_entry.path().filename().string());
        if (is_verbose) { std::cout << print_pkgit << "copied include: " << dir_entry << "\n"; }
      } else { /*std::cout << print_pkgit << "include already exists: " << dir_entry << "\n";*/ }
    }
  }
}
