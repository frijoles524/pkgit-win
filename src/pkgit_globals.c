int is_verbose = 1;
int is_forced = 0;

//void init_vars(void) {
//	char *home = getenv("HOME");
//	if (home) snprintf(home_dir, MAX_PATH_LEN, "%s", home);
//	else snprintf(home_dir, MAX_PATH_LEN, "/root");
//
//	is_root_config = file_exists(root_config);
//
//	if (is_root_config) snprintf(config_dir, MAX_PATH_LEN, "/etc/pkgit");
//	else snprintf(config_dir, MAX_PATH_LEN, "%s/.config/pkgit", home_dir);
//
//	snprintf(config_file, MAX_PATH_LEN, "%s/init.lua", config_dir);
//	snprintf(repo_file, MAX_PATH_LEN, "%s/repos.lua", config_dir);
//
//	config_exists = file_exists(root_config) || file_exists(home_config());
//
//	snprintf(bin, MAX_PATH_LEN, "%s/.local/bin", home_dir);
//	snprintf(lib, MAX_PATH_LEN, "%s/.local/lib", home_dir);
//	snprintf(inc, MAX_PATH_LEN, "%s/.local/include", home_dir);
//	snprintf(src, MAX_PATH_LEN, "%s/.local/share/pkgit", home_dir);
//
//	install_directories[0] = config_dir;
//	install_directories[1] = strdup(get_install_dir("bin"));
//	install_directories[2] = strdup(get_install_dir("lib"));
//	install_directories[3] = strdup(get_install_dir("inc"));
//	install_directories[4] = strdup(get_install_dir("src"));
//}
