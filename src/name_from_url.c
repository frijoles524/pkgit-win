#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "name_from_url.h"

char* name_from_url(const char *url) {
    size_t len = strlen(url);
    const char *end = url + len;
    while (end > url && *(end-1) == '/') {
        end--;
    }
    const char *last_slash = end;
    while (last_slash > url && *(last_slash-1) != '/') {
        last_slash--;
    }
    if (last_slash < end) {
        return strndup(last_slash, end - last_slash);
    } else {
        return strndup(url, end - url);
    }
}
