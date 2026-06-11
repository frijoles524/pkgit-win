#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#include "is_updated.h"
#include "cmd_out.h"
#include "vars.h"

bool is_updated(const char *src) {
    if (src && strlen(src) > 0 && chdir(src) != 0) {
      return false;
    }

    char *output = cmd_out("git pull");
    bool result = (strstr(output, "Already up to date.") != NULL);
    free(output);
    return result;
}