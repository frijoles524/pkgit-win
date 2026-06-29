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

#include "cla_parse.h"
#include "pkgit_string.h"
#include "pkgit_globals.h"

int main(int argc, char** argv) {
	//slc_cat(BOLD_YELLOW, slc_from_cstr("["))
	//slc_cat(BOLD_MAGENTA, slc_from_cstr("pkgit"))
	//slc_cat(BOLD_MAGENTA, slc_from_cstr("]"))
	//COLOR_RESET
	str_slc hello_world = {"", 0};
	slc_cat(slc_from_cstr("hello"), slc_from_cstr(" world"), &hello_world);
	str_slc test = {"hello", 5};
	printf("%.*s\n", str_fmt(test));
	printf("%.*s%s%.*s\n", str_fmt(BOLD_YELLOW), "TEST", str_fmt(COLOR_RESET));
	printf("length: %zu\nstring: %.*s\n", hello_world.len, str_fmt(hello_world));
	str_slc hello_world_ext = {"", 0};
	slc_cat(slc_from_cstr(hello_world.data), slc_from_cstr("!!!"), &hello_world_ext);
	printf("length: %zu\nstring: %.*s\n", hello_world_ext.len, str_fmt(hello_world_ext));
	return 0;
}
