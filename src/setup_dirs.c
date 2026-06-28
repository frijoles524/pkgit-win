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

#include <string.h>
#include <sys/stat.h>

#include "setup_dirs.h"
#include "vars.h"

void setup_dirs(void) {
	mkdir_p(config_dir);
	for (int i = 0; i < 5; i++) {
		if (install_directories[i] && strlen(install_directories[i]) <= 0) continue;
		if (file_exists(install_directories[i])) continue;
		mkdir_p(install_directories[i]);
	}
}