#include <stdio.h>

#include "list_pkgs.h"
#include "lua_state.h"

void list_pkgs() {
    init_lua_state();
    cache_repos();
    for (size_t i = 0; i < cached_repos_count; i++) {
        printf("%s\n", cached_repos[i].source_key);
    }
}