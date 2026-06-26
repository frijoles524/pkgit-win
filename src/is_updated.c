#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#include "is_updated.h"

#include "cmd_out.h"
#include "lua_state.h"

bool is_updated(const char *src) {
  bool result = false;
  if (src && strlen(src) > 0 && chdir(src) != 0) return result;
  char *bldit_pkgver = bldit_pkg_getver();
  char *git_tag = cmd_out("git tag | tail -n 1");
  result = (strstr(git_tag, bldit_pkgver) != NULL);
  char *git_pull = cmd_out("git pull");
  result = (strstr(git_pull, "Already up to date.") != NULL);
  free(git_tag);
  free(git_pull);
  return result;
}

bool is_bldit_usable() {
  char* bldit_version = bldit_getver();
  if (strcmp(bldit_version, version) == 0) return true;
  bool prev_pass = false;
  for (size_t i = 0; i < strlen(bldit_version); i++) {
    if (bldit_version[i] == '.') continue;
    if ((bldit_version[i] - '0') <= (version[i] - '0')) {
      prev_pass = ((bldit_version[i] - '0') != (version[i] - '0'));
      continue;
    } else return prev_pass;
  }
  return true;
}