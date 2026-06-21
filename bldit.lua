bldit_version   = "0.1.3"
package_version = "0.1.3"

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
      os.execute("make")
    end,
    install = function()
      os.execute("make install")
    end,
    uninstall = function()
      os.execute("make uninstall")
    end,
  },
  quiet = {
    build = function()
      local output = io.popen("make"):read("*a")
    end,
    install = function()
      local output = io.popen("make install"):read("*a")
    end,
    uninstall = function()
      local output = io.popen("make uninstall"):read("*a")
    end,
  },
}
