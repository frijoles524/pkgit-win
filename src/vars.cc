#include <filesystem>
#include <map>
#include <string>

#include "vars.hh"

std::map<std::string, std::string> repos;
std::map<std::string, std::string> install_directories;

bool is_symlink_install = false;
bool is_verbose = false;

const std::string home_dir = std::getenv("HOME");

const std::string root_config = "/etc/pkgit/init.lua";
const std::string home_config = home_dir + "/.config/pkgit";
bool is_root_config = std::filesystem::exists(root_config);

const std::string config_dir = is_root_config ? "/etc/pkgit" : home_dir + "/.config/pkgit";
const std::string config_file = config_dir + "/init.lua";
const std::string repo_file = config_dir + "/repos.lua";

bool config_exists = std::filesystem::exists(root_config) || std::filesystem::exists(home_config);

const std::string bin = config_exists ? install_directories["bin"] : home_dir + "/.local/bin";
const std::string lib = config_exists ? install_directories["lib"] : home_dir + "/.local/lib";
const std::string include = config_exists ? install_directories["include"] : home_dir + "/.local/include";
const std::string pkgblds = config_exists ? install_directories["pkgblds"] : home_dir + "/.local/share/pkgit";
const std::string all_dirs[] = {
	config_dir,
	install_directories["bin"],
	install_directories["lib"],
	install_directories["include"],
	install_directories["pkgblds"]
};

// version
const std::string version = "0.0.0";

// colors
const std::string red = "\e[0;31m";
const std::string green = "\e[0;32m";
const std::string yellow = "\e[0;33m";
const std::string blue = "\e[0;34m";
const std::string magenta = "\e[0;35m";
const std::string cyan = "\e[0;36m";
const std::string gray = "\e[0;37m";
// bright
const std::string bright_red = "\e[0;91m";
const std::string bright_green = "\e[0;92m";
const std::string bright_yellow = "\e[0;93m";
const std::string bright_blue = "\e[0;94m";
const std::string bright_magenta = "\e[0;95m";
const std::string bright_cyan = "\e[0;96m";
const std::string bright_gray = "\e[0;97m";
// bold
const std::string bold_red = "\e[1;31m";
const std::string bold_green = "\e[1;32m";
const std::string bold_yellow = "\e[1;33m";
const std::string bold_blue = "\e[1;34m";
const std::string bold_magenta = "\e[1;35m";
const std::string bold_cyan = "\e[1;36m";
const std::string bold_gray = "\e[1;37m";
const std::string bold_white = "\e[1;38m";
// bold bright
const std::string bold_bright_red = "\e[1;91m";
const std::string bold_bright_green = "\e[1;92m";
const std::string bold_bright_yellow = "\e[1;93m";
const std::string bold_bright_blue = "\e[1;94m";
const std::string bold_bright_magenta = "\e[1;95m";
const std::string bold_bright_cyan = "\e[1;96m";
const std::string bold_bright_gray = "\e[1;97m";
// italic
const std::string italic = "\e[3m";
// reset
const std::string color_reset = "\e[0m";

const std::string print_pkgit = bold_yellow + "[" + bold_magenta + "pkgit" + bold_yellow + "]\t" + color_reset;
const std::string print_skipped = print_pkgit + blue + "[SKIPPED]\t" + color_reset;
const std::string print_error = print_pkgit + red + "[ERROR]\t" + color_reset;
