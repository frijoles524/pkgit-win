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

#ifndef PKGIT_STRING_H
#define PKGIT_STRING_H
#include <stddef.h>

typedef struct {
	char* data;
	size_t len;
} str_slc;

typedef struct {
	char* data;
	size_t cap;
	size_t len;
} str;

#define str_fmt(slice) (int)((slice).len), (slice).data
// Usage: printf("%.*s", format_string(slice));

void print_slice(str_slc s);
str_slc slc_from_cstr(char* cstr);
#define slc_from_cstr(cstr) slc_from_cstr(cstr)
int slc_eq(str_slc a, str_slc b);
int slc_starts_with(str_slc slice, str_slc prefix);
str_slc slc_take(str_slc slice, size_t n);
str_slc slc_drop(str_slc slice, size_t n);
str_slc slc_trim(str_slc slice);
void slc_cat(str_slc first, str_slc second, str_slc *new_slc);
//#define slc_cat(first, second, new_slc) slc_cat(first, second, *new_slc)

#endif
