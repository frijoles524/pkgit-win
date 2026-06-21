#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "build.h"
#include "lua_build.h"
#include "vars.h"

void build(Pkg pkg) {
  char cwd[MAX_PATH_LEN];
  getcwd(cwd, MAX_PATH_LEN);
  if (strcmp(pkg.src, cwd) != 0 && !pkg.is_local) chdir(pkg.src);

  if (lua_build(pkg.name, pkg.target, pkg.is_local ? cwd : pkg.src)) return;
  printf("%s no usable build system was found\n", print_error);
  exit(EXIT_FAILURE);
}