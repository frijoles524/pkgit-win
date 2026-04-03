#include <iostream>

#include "add_repo.hh"
#include "fetch_src.hh"
#include "build.hh"
#include "copy_install.hh"
#include "install_pkg.hh"
#include "link_install.hh"
#include "vars.hh"

void install_pkg(Pkg pkg) {
  std::cout << print_pkgit << "fetching source..." << std::endl;
  fetch_src(pkg);

  std::cout << print_pkgit << "building..." << std::endl;
  build(pkg.src.c_str());

  std::cout << print_pkgit << "installing..." << std::endl;
  if (is_symlink_install) { link_install(pkg.src); }
  else { copy_install(pkg.src); }

  bool repo_exists = false;
  for (auto repo : repos) {
    if (repo.first == pkg.name) {
      repo_exists = true;
    }
  }

  if (!repo_exists) {
    std::cout << print_pkgit << "adding repository..." << std::endl;
    add_repo(pkg.url, pkg.name);
    std::cout << print_pkgit << "done!" << std::endl;
  } else {
    std::cout << print_pkgit << "repo already exists, done!" << std::endl;
  }
}
