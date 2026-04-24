local prefix = "/home/dacc/pkgit"
local install_directories = {
	bin			= prefix.."/bin",
	include	= prefix.."/include",
	lib			= prefix.."/lib",
	src			= prefix.."/src",
}

local repositories = {
	pkgit = { "https://git.symlinx.net/pkgit" },
	beaker = {
		"https://git.symlinx.net/pkgit",
		dependencies = {}
		build = function()
			os.execute("make")
		end,
		pre_install = function() end
		install = funciton()
			os.execute("make install INSTALL_PREFIX="..prefix)
		end,
		post_install = function() end
	},
}

local build_systems = {
	Makefile = {
		build = function()
			os.execute("make")
		end,
		install = function()
			os.execute("make install")
		end,
	},
}
