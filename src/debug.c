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


#include "globs.h"

void debug_print_inst_dirs(void) {
	printf("PREFIX:\t");
	str_println(&inst_dirs.prefix);
	printf("ID_BIN:\t");
	str_println(&inst_dirs.bin);
	printf("ID_LIB:\t");
	str_println(&inst_dirs.lib);
	printf("ID_INCLUDE:\t");
	str_println(&inst_dirs.include);
	printf("ID_SRC:\t");
	str_println(&inst_dirs.src);
}
