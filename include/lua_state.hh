#ifndef LUA_STATE
#define LUA_STATE

#include <unordered_map>
#include <string>
extern "C" {
#include <luajit-2.1/lua.h>
#include <luajit-2.1/lauxlib.h>
#include <luajit-2.1/lualib.h>
}

typedef struct {
	std::string key;
	std::string value;
} url;

typedef struct {
	std::string url;
	std::string version;
} dependency;

typedef struct {
	url source;
	std::unordered_map<std::string, dependency> dependencies;
	int build;
	int pre_install;
	int install;
	int post_install;
} repo;

extern std::unordered_map<std::string, std::string> cached_install_directories;
extern std::unordered_map<std::string, repo> cached_repos;
extern std::unordered_map<std::string, int> cached_build_systems;


void init_lua_state();
void free_lua_state();
lua_State* get_lua_state();

void cache_install_directories();
bool repo_build(const char* repoository);
bool bldit(const char* target);
bool config_build(const char* path);
void cache_repos();
void cache_build_systems();

#endif