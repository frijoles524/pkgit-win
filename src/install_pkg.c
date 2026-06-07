#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "add_repo.h"
#include "build.h"
#include "fetch_src.h"
#include "install_pkg.h"
// #include "copy_install.h"
// #include "link_install.h"
#include "lua_state.h"
#include "name_from_url.h"
#include "vars.h"

void install_pkg(Pkg pkg) {
  if (!pkg.is_local) {
    printf("%sfetching %s%s%s\n", print_pkgit, green, pkg.name, color_reset);
    fetch_src(pkg);
    if (is_verbose)
      printf("%sfetched %s%s%s\n", print_pkgit, green, pkg.name, color_reset);
  }

  printf("%sbuilding %s%s%s\n", print_pkgit, green, pkg.name, color_reset);
  build(pkg);
  if (is_verbose)
    printf("%sbuilt %s%s%s\n", print_pkgit, green, pkg.name, color_reset);

  printf("%sinstalling %s%s%s\n", print_pkgit, green, pkg.name, color_reset);
  bool install_success = false;
  if (!install_success && repo_install(pkg.name))
    install_success = true;
  if (!install_success && bldit_install(pkg.target))
    install_success = true;
  if (!install_success && config_install(pkg.src))
    install_success = true;
  if (!install_success) {
    printf("%sno install function availible for package: %s\n", print_error,
           pkg.name);
    return;
  }
  // is_auto_installed = true;
  // if (is_auto_installed) {
  //   if (is_symlink_install) {
  //       link_install(pkg.src);
  //   } else {
  //       copy_install(pkg.src);
  //   }
  // }
  printf("%sinstalled %s%s%s\n", print_success, green, pkg.name, color_reset);

  bool repo_exists = false;
  for (size_t i = 0; i < cached_repos_count; i++) {
    char *repo_name = name_from_url(cached_repos[i].source_value);
    if (strcmp(repo_name, pkg.name) == 0) {
      repo_exists = true;
    }
    free(repo_name);
  }

  if (!repo_exists) {
    printf("%sadding %s%s%s\n", print_pkgit, green, pkg.name, color_reset);
    if (pkg.url && strlen(pkg.url) > 0) {
      add_repo(pkg.url, pkg.name);
    }
    printf("%sadded %s%s%s\n", print_pkgit, green, pkg.name, color_reset);
  } else {
    if (is_verbose)
      printf("%srepo already exists, done\n", print_pkgit);
  }
}