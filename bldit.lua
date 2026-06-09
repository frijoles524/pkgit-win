bldit_version   = "0.1.3"
package_version = "0.1.3"

global_dependencies = {
  luajit = {
    url = "https://luajit.org/git/luajit.git",
    version = "v2.1",
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
  },
  quiet = {
    build = function()
      local output = io.popen("make"):read("*a")
    end,
    install = function()
      local output = io.popen("make install"):read("*a")
    end,
  },
}
