bldit_version = "0.0.0"
package_version = "0.0.0"

targets = {
	default = {
		build = function(name)
			os.execute("make")
		end,
		install = function(prefix)
			os.execute("make install")
		end,
	},
	quiet = {
		build = function(name)
			local output = io.popen("make"):read("*a")
		end,
		install = function(prefix)
			local output = io.popen("make install"):read("*a")
		end,
	},
}
