#include <iostream>
#include <unordered_map>

#include "cla_parse.hh"
#include "setup_pkgit.hh"
#include "lua_state.hh"

int main(int argc, char *argv[]) {
	setup_pkgit();
	//init_lua_state();
	//repo_build("omnisearch");
	cla_parse(argc, argv);
	free_lua_state();
	return 0;
}
