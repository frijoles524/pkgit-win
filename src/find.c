#include <stdio.h>
#include <string.h>

#include "list_pkgs.h"
#include "lua_state.h"

void find(const char* arg) {
  init_lua_state();
  cache_repos();
  if (!arg) {
    for (size_t i = 0; i < cached_repos_count; i++) {
      printf("%s\n", cached_repos[i].source_key);
    }
    return;
  }
  for (size_t i = 0; i < cached_repos_count; i++) {
    if (!strstr(cached_repos[i].source_key, arg)) continue;
    printf("%s\n", cached_repos[i].source_key);
  }
}
