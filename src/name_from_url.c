#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "name_from_url.h"

char* name_from_url(const char *url) {
    size_t len = strlen(url);
    char* after_git = strdup(url);
    const char *end = after_git + len;
    while (end > after_git && *(end-1) == '/') {
        end--;
    }
    const char *last_slash = end;
    while (last_slash > after_git && *(last_slash-1) != '/') {
        last_slash--;
    }
    if (last_slash < end) {
        return strndup(last_slash, end - last_slash);
    } else {
        return strndup(after_git, end - after_git);
    }
}
