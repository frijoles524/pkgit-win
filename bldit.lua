bldit_version   = "1.0.0"
package_version = "1.0.0"

global_dependencies = {
  luajit = {
    url = "https://luajit.org/git/luajit.git",
    version = "v2.1",
    target = "default",
  },
  git = {
    url = "https://github.com/git/git",
    version = "HEAD",
    target = "default",
  },
}

targets = {
  default = {
    build = function()
      return os.execute("make")
    end,
    install = function()
      return os.execute("make install PREFIX="..prefix)
    end,
    uninstall = function()
      return os.execute("make uninstall PREFIX="..prefix)
    end,
  },
  quiet = {
    build = function()
      return os.execute("make &>/tmp/pkgit_build.log")
    end,
    install = function()
      return os.execute("make install PREFIX="..prefix.." &>/tmp/pkgit_build.log")
    end,
    uninstall = function()
      return os.execute("make uninstall PREFIX="..prefix.." &>/tmp/pkgit_build.log")
    end,
  },
}
