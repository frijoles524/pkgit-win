#include <stdio.h>

#include "help.h"
#include "vars.h"

void help() {
  if (is_verbose) {
    printf("%s      ,        \n", bold_magenta);
    printf("%s     / \\       \n", bold_magenta);
    printf("%s_.--'   '--._  \n", bold_magenta);
    printf("%s`'--,   ,--'`  %spkgit %s%s- package it!%s\n", bold_magenta, color_reset, italic, gray, color_reset);
    printf("%s _-  %s\\ /%s  -_   %s%s%s\n", bold_yellow, bold_magenta, bold_yellow, magenta, version, color_reset);
    printf("%s'-_   %s'%s   _-'  \n", bold_yellow, bold_magenta, bold_yellow);
    printf("%s   `'-.-'`     %s\n", bold_yellow, color_reset);
  } else {
    printf("pkgit %s%s- package it!%s\n", italic, gray, color_reset);
    printf("%s%s%s\n", magenta, version, color_reset);
  }
  printf("\n");
  printf("%ssubcommands%s:\n", red, color_reset);
  printf("%s  %sa%s,   %sadd %s[url]              %s# add a repo\n", color_reset, green, color_reset, yellow, blue, gray);
  printf("%s  %sb%s,   %sbuild %s[path]           %s# build a package\n", color_reset, green, color_reset, yellow, blue, gray);
  printf("%s  %sd%s,   %sdeclare                %s# install all packages\n", color_reset, green, color_reset, yellow, gray);
  printf("%s  %ss%s,   %ssearch %s[pkgs]          %s# find a package from your repos\n", color_reset, green, color_reset, yellow, blue, gray);
  printf("%s  %si%s,   %sinstall %s[pkgs, urls]   %s# install a package/repo\n", color_reset, green, color_reset, yellow, blue, gray);
  printf("%s  %sr%s,   %sremove %s[pkgs]          %s# remove an installed package\n", color_reset, green, color_reset, yellow, blue, gray);
  printf("%s  %sl%s,   %slist                   %s# list all installed packages\n", color_reset, green, color_reset, yellow, gray);
  printf("%s  %su%s,   %supdate                 %s# update all installed packages\n", color_reset, green, color_reset, yellow, gray);
  printf("\n");
  printf("%sflags%s:\n", red, color_reset);
  printf("%s  %s-h%s,  %s--help                 %s# display this help message\n", color_reset, green, color_reset, yellow, gray);
  printf("%s  %s-q%s,  %s--quiet                %s# run without logging to terminal\n", color_reset, green, color_reset, yellow, gray);
  printf("%s  %s-f%s,  %s--force                %s# force an operation like 'install'\n", color_reset, green, color_reset, yellow, gray);
  printf("%s  %s-v%s,  %s--version              %s# display version number\n", color_reset, green, color_reset, yellow, gray);
  printf("%s  %s-c%s,  %s--check                %s# run package checks\n", color_reset, green, color_reset, yellow, gray);
}
