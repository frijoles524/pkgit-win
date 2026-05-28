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
	}
}
