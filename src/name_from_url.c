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
#include <string.h>
#include <stdlib.h>

#include "name_from_url.h"

char* name_from_url(const char *url) {
	size_t len = strlen(url);
	char* after_git = strdup(url);
	const char *end = after_git + len;
	while (end > after_git && *(end-1) == '/')
		end--;
	if (strncmp(end-4, ".git", 4) == 0)
		end -= 4;
	const char *last_slash = end;
	while (last_slash > after_git && *(last_slash-1) != '/')
		last_slash--;
	if (last_slash < end) return strndup(last_slash, end - last_slash);
	else return strndup(after_git, end - after_git);
}
