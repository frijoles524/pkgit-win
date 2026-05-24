#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "update_all.h"
#include "create_pkg.h"
#include "update_pkg.h"
#include "lua_state.h"

void update_all() {
    init_lua_state();
    cache_repos();
    for (size_t i = 0; i < cached_repos_count; i++) {
        Pkg pkg = create_pkg(cached_repos[i].source_value, "default");
        update_pkg(pkg);
    }
}