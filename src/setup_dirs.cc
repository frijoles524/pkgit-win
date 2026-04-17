#include <filesystem>
#include <iostream>

#include "setup_dirs.hh"
#include "vars.hh"

void setup_dirs() {
	std::filesystem::create_directories(config_dir);
	std::filesystem::create_directories(install_directories["bin"]);
	std::filesystem::create_directories(install_directories["lib"]);
	std::filesystem::create_directories(install_directories["include"]);
	std::filesystem::create_directories(install_directories["pkgblds"]);
	//for(unsigned int i = 0; i < sizeof(all_dirs)/sizeof(all_dirs[0]); i++) {
		//std::filesystem::create_directories(all_dirs[i]);
	//}
}
