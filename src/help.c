/*

  pkgit - package it!

  Copyright (C) 2026 dacctal
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/

#include <stdio.h>

#include "help.h"
#include "vars.h"

void help(void) {
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
  printf("%ssubcommand flags%s:\n", red, color_reset);
  printf("%s  %s-a%s,  %s--add %s[url]            %s# add a repo\n", color_reset, green, color_reset, yellow, blue, gray);
  printf("%s  %s-b%s,  %s--build %s[path]         %s# build a package\n", color_reset, green, color_reset, yellow, blue, gray);
  printf("%s  %s-d%s,  %s--declare              %s# install all packages\n", color_reset, green, color_reset, yellow, gray);
  printf("%s  %s-s%s,  %s--search %s[pkgs]        %s# find a package from your repos\n", color_reset, green, color_reset, yellow, blue, gray);
  printf("%s  %s-i%s,  %s--install %s[pkgs, urls] %s# install a package/repo\n", color_reset, green, color_reset, yellow, blue, gray);
  printf("%s  %s-r%s,  %s--remove %s[pkgs]        %s# remove an installed package\n", color_reset, green, color_reset, yellow, blue, gray);
  printf("%s  %s-l%s,  %s--list                 %s# list all installed packages\n", color_reset, green, color_reset, yellow, gray);
  printf("%s  %s-u%s,  %s--update               %s# update all installed packages\n", color_reset, green, color_reset, yellow, gray);
  printf("%s  %s-h%s,  %s--help                 %s# display this help message\n", color_reset, green, color_reset, yellow, gray);
  printf("%s  %s-v%s,  %s--version              %s# display version number\n", color_reset, green, color_reset, yellow, gray);
  printf("%s  %s-c%s,  %s--check                %s# run package checks\n", color_reset, green, color_reset, yellow, gray);
  printf("\n");
  printf("%smodifier flags%s:\n", red, color_reset);
  printf("%s  %s-q%s,  %s--quiet                %s# run without logging to terminal\n", color_reset, green, color_reset, yellow, gray);
  printf("%s  %s-f%s,  %s--force                %s# force an operation like 'install'\n", color_reset, green, color_reset, yellow, gray);
}
