#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "create_pkg.h"
#include "lua_state.h"
#include "name_from_url.h"
#include "vars.h"

Pkg create_pkg(const char *arg, const char *target) {
  Pkg pkg = {0};
  pkg.target = target;
  pkg.ver = "HEAD";
  pkg.is_local = false;

  for (int i = 0; i < cached_repos_count; i++) {
    if (strcmp(cached_repos[i].source_key, arg) == 0) {
      pkg.ver = cached_repos[i].version;
    }
  }

  char *new_arg = strdup(arg);
  if (!new_arg) exit(EXIT_FAILURE);
  char *argver = strchr(new_arg, '@');
  if (argver) {
    pkg.ver = argver + 1;
    *argver = '\0';
  }

  bool is_in_repos = false;
  for (size_t i = 0; i < cached_repos_count; i++) {
    if (strcmp(new_arg, cached_repos[i].source_key) == 0) {
      is_in_repos = true;
      break;
    }
  }

  if (strncmp(new_arg, "http", 4) == 0) {
    pkg.url = strdup(new_arg);
    pkg.name = name_from_url(new_arg);
  } else if (strcmp(new_arg, ".") == 0) {
    pkg.url = "";
    getcwd(pkg.src, MAX_PATH_LEN);
    pkg.name = name_from_url(pkg.url);
    pkg.is_local = true;
  } else if (is_in_repos) {
    for (size_t i = 0; i < cached_repos_count; i++) {
      if (strcmp(new_arg, cached_repos[i].source_key) == 0) {
        pkg.url = strdup(cached_repos[i].source_value);
        break;
      }
    }
    pkg.name = strdup(new_arg);
  } else {
    printf("%s'%s' is not a valid package\n", print_error, new_arg);
    exit(EXIT_FAILURE);
  }

  if (strlen(pkg.name) > 4 && strncmp(pkg.name + strlen(pkg.name) - 4, ".git", 4) == 0) {
    pkg.name[strlen(pkg.name) - 4] = '\0';
  }

  cache_install_directories();
  if (!pkg.is_local) {
    char src_dir[MAX_PATH_LEN];
    snprintf(src_dir, sizeof(src_dir), "%s/%s/%s", get_install_dir("src"),
             pkg.name, pkg.ver);
    snprintf(pkg.src, MAX_PATH_LEN, "%s", src_dir);
  }

  return pkg;
}