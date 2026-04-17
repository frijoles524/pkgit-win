#ifndef VARS
#define VARS

#include <string>
#include <filesystem>
#include <map>

extern std::map<std::string, std::string> repos;
extern std::map<std::string, std::string> install_directories;

extern bool is_symlink_install;
extern bool is_verbose;
extern bool config_exists;

struct Pkg {
  std::string url;
  std::string name;
  std::string ver;
  std::filesystem::path src;
};

extern const std::string home_dir;

extern const std::string root_config;
extern bool is_root_config;
extern const std::string config_dir;
extern const std::string config_file;
extern const std::string repo_file;

extern const std::string bin;
extern const std::string lib;
extern const std::string include;
extern const std::string pkgblds;
extern const std::string all_dirs[5];

// version
extern const std::string version;

// colors
extern const std::string red;
extern const std::string green;
extern const std::string yellow;
extern const std::string blue;
extern const std::string magenta;
extern const std::string cyan;
extern const std::string gray;
// bright
extern const std::string bright_red;
extern const std::string bright_green;
extern const std::string bright_yellow;
extern const std::string bright_blue;
extern const std::string bright_magenta;
extern const std::string bright_cyan;
extern const std::string bright_gray;
// bold
extern const std::string bold_red;
extern const std::string bold_green;
extern const std::string bold_yellow;
extern const std::string bold_blue;
extern const std::string bold_magenta;
extern const std::string bold_cyan;
extern const std::string bold_gray;
extern const std::string bold_white;
// bold bright
extern const std::string bold_bright_red;
extern const std::string bold_bright_green;
extern const std::string bold_bright_yellow;
extern const std::string bold_bright_blue;
extern const std::string bold_bright_magenta;
extern const std::string bold_bright_cyan;
extern const std::string bold_bright_gray;
// italic
extern const std::string italic;
// reset
extern const std::string color_reset;

extern const std::string print_pkgit;
extern const std::string print_skipped;
extern const std::string print_error;

#endif
