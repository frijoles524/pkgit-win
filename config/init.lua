local home = os.getenv("HOME")
local prefix = home.."/.local"
install_directories = {
  bin	    = prefix.."/bin",
  include	= prefix.."/include",
  lib	    = prefix.."/lib",
  src     = prefix.."/share/pkgit",
}

repositories = {
  pkgit = {
    url = "https://git.symlinx.net/pkgit",
  },
}

build_systems = {
  ["Makefile"] = {
    build = function()
      os.execute("make")
    end,
    install = function()
      os.execute("make install PREFIX="..prefix)
    end,
    uninstall = function()
      os.execute("make uninstall PREFIX="..prefix)
    end,
  },
  ["meson.build"] = {
    build = function()
      os.execute("meson setup build --prefix "..prefix.." && meson compile -C build")
    end,
    install = function()
      os.execute("cd build && meson install")
    end,
    uninstall = function()
      os.execute("cd build && ninja uninstall")
    end,
  },
  ["CMakeLists.txt"] = {
    build = function()
      os.execute("cmake -B build")
      os.execute("cmake --build build")
    end,
  },
}