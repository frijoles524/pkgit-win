#include <filesystem>
#include <iostream>

#include "setup_dirs.hh"
#include "vars.hh"

void setup_dirs() {
	std::filesystem::create_directories(config_dir);
	for (auto i: install_directories) {
		if (std::filesystem::exists(i.second)) continue;
		if (i.second == "") continue;
		std::filesystem::create_directories(i.second);
	}
	//for(unsigned int i = 0; i < sizeof(all_dirs)/sizeof(all_dirs[0]); i++) {
	//	std::filesystem::create_directories(all_dirs[i]);
	//}
}
