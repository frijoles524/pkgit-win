#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "create_pkg.h"
#include "files.h"
#include "lua_state.h"
#include "name_from_url.h"
#include "vars.h"

Pkg create_pkg(const char *arg) {
  Pkg pkg = {0};
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
  char *argtrg = strchr(new_arg, ',');
  if (argver) {
    pkg.ver = argver + 1;
    *argver = '\0';
  }
  if (argtrg) {
    pkg.target = argtrg + 1;
    *argtrg = '\0';
  } else {
    pkg.target = "default";
  }

  bool is_in_repos = false;
  for (size_t i = 0; i < cached_repos_count; i++) {
    if (strcmp(new_arg, cached_repos[i].source_key) == 0) {
      is_in_repos = true;
      break;
    }
  }

  cache_install_directories();
  bool is_installed_locally = false;
  char dest_dir[MAX_PATH_LEN];
  if (new_arg[0] == '.') {
  	char cwd[MAX_PATH_LEN];
  	getcwd(cwd, MAX_PATH_LEN);
	  snprintf(dest_dir, sizeof(dest_dir), "%s/%s",
	    get_install_dir("src"), name_from_url(cwd));
  } else {
	  snprintf(dest_dir, sizeof(dest_dir), "%s/%s",
	    get_install_dir("src"), new_arg);
  }
  if (is_directory(dest_dir)) is_installed_locally = true;

  if (strncmp(new_arg, "http", 4) == 0) {
    pkg.url = strdup(new_arg);
    pkg.name = name_from_url(new_arg);
  } else if (strcmp(new_arg, ".") == 0) {
    pkg.url = "";
    char cwd[MAX_PATH_LEN];
    getcwd(cwd, MAX_PATH_LEN);
    pkg.name = name_from_url(cwd);
    pkg.is_local = true;
  } else if (is_in_repos) {
    for (size_t i = 0; i < cached_repos_count; i++) {
      if (strcmp(new_arg, cached_repos[i].source_key) == 0) {
        pkg.url = strdup(cached_repos[i].source_value);
        break;
      }
    }
    pkg.name = strdup(new_arg);
  } else if (is_installed_locally) {
    pkg.url = "";
    pkg.name = name_from_url(dest_dir);
    pkg.is_local = true;
  } else {
    printf("%s '%s' is not a valid package\n", print_error, new_arg);
    exit(EXIT_FAILURE);
  }

  if (strlen(pkg.name) > 4 &&
  strncmp(pkg.name + strlen(pkg.name) - 4, ".git", 4) == 0)
    pkg.name[strlen(pkg.name) - 4] = '\0';

  char src_dir[MAX_PATH_LEN];
  if (pkg.is_local) {
	  snprintf(src_dir, sizeof(src_dir), "%s/%s",
	    get_install_dir("src"), pkg.name);
  } else {
	  snprintf(src_dir, sizeof(src_dir), "%s/%s/%s",
	    get_install_dir("src"), pkg.name, pkg.ver);
  }
  snprintf(pkg.src, MAX_PATH_LEN, "%s", src_dir);

  return pkg;
}