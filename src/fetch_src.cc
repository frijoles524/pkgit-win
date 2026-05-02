#include <stdlib.h>
#include <iostream>
#include "fetch_src.hh"
#include "fetch_git.hh"

void fetch_src(Pkg pkg) {
  std::cout << print_pkgit << "target source directory: " << pkg.src << std::endl;
  if (std::filesystem::exists(pkg.src)) {
    std::cout << print_pkgit << pkg.src << " already exists. deleting..." << std::endl;
    std::filesystem::remove_all(pkg.src);
  }
  if (pkg.url == "") {
    std::cout << print_pkgit << "creating directory " << pkg.src << "..." << std::endl;
    std::filesystem::create_directories(pkg.src);
    return;
  }
  else if (fetch_git(pkg) == 0) {
    std::cout << print_pkgit << "cloned into " << pkg.src << "..." << std::endl;
    return;
  }
  else {
    std::cout << print_error << "no fetch methods worked." << std::endl;
    exit(EXIT_FAILURE);
  }
}
