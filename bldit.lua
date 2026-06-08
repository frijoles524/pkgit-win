bldit_version   = "0.1.2"
package_version = "0.1.2"

global_dependencies = {
  luajit = {
    url = "https://luajit.org/git/luajit.git",
    version = "v2.1",
    target = "default",
  },
}

targets = {
  default = {
    dependencies = {
      pipemixer = {
        url = "https://github.com/heather7283/pipemixer",
        version = "HEAD",
        target = "default",
      },
    },
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
