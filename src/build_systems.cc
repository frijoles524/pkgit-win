#include <filesystem>

#include "../include/toml.hh"

#include "vars.cc"

void autogen_build() {
  system("./autogen.sh");
  system("make");
}

void autotools_build() {
  system("./configure");
  for (auto const& dir_entry : fs::directory_iterator(fs::current_path().string())) {
    if (dir_entry.path().filename() == "CMakeLists.txt") {
      fs::create_directories("build");
      fs::current_path("build");
      system("cmake ..");
    }
  }
  for (auto const& dir_entry : fs::directory_iterator(fs::current_path().string())) {
    if (dir_entry.path().filename() == "Makefile") {
      system("make");
    }
  }
}

void bldit_build() {
  system("./bldit");
}

void buildsh_build() {
  system("./build.sh");
}

void compilesh_build() {
  system("./compile.sh");
}

void cargo_build() {
  system("cargo build --release");
}

void cmake_build() {
  fs::create_directories("build");
  fs::current_path("build");
  system("cmake ..");
  system("make");
}

void go_build() {
  for (auto const& dir_entry : fs::directory_iterator(fs::current_path().string())) {
    if (dir_entry.path().filename() == "main.go") {
      system("go build main.go");
    }
  }
}

void gradle_build() {
  system("gradle build");
}

void make_build() {
  for (auto const& dir_entry : fs::directory_iterator(fs::current_path().string())) {
    if (dir_entry.path().filename() == "autogen.sh") {
      autogen_build();
      return;
    }
  }
  for (auto const& dir_entry : fs::directory_iterator(fs::current_path().string())) {
    if (dir_entry.path().filename() == "configure.ac") {
      autotools_build();
      return;
    }
  }
  system("make");
}

void meson_build() {
  system("meson setup --wipe build");
  system("meson compile -C build");
}

void ninja_build() {
  system("ninja");
}

void nim_build() {
  system("nimble build");
}

void pnpm_build() {
  system("pnpm install");
  system("pnpm run build");
}

void python_build() {
  toml::parse_result pyproject_toml = toml::parse_file("pyproject.toml");
  std::string pypkg = pyproject_toml["project"]["name"].ref<std::string>();
  std::string commands[] = {
    "export PIPX_BIN_DIR=" + bin,
    "pipx install " + pypkg
  };
  for (int i = 0; i < sizeof(commands)/sizeof(commands[0]); i++) {
    system(commands[i].c_str());
  }
}

void zig_build() {
  system("zig build");
}
