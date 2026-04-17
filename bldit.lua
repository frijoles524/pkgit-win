bldit_version = "0.0.0"
package_version = "0.0.0"

global_dependencies = {}

targets = {
	default = {
		dependencies = {}
		compile = function() os.execute("make") end
		pre_install = function() end
		install = function() os.execute("make install") end
		post_install = function() end
	}
}
