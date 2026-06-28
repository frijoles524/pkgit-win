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
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "cmd_out.h"

char* cmd_out(const char *cmd) {
    FILE *pipe = popen(cmd, "r");
    if (!pipe) {
        return strdup("");
    }

    char buffer[128];
    size_t total_size = 0;
    size_t capacity = 256;
    char *result = malloc(capacity);
    if (!result) {
        pclose(pipe);
        return strdup("");
    }
    result[0] = '\0';

    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        size_t len = strlen(buffer);
        if (total_size + len + 1 > capacity) {
            capacity *= 2;
            char *new_result = realloc(result, capacity);
            if (!new_result) {
                free(result);
                pclose(pipe);
                return strdup("");
            }
            result = new_result;
        }
        strcat(result, buffer);
        total_size += len;
    }

    pclose(pipe);
    return result;
}