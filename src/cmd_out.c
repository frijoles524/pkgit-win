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