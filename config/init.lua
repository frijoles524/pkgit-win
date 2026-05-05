local home = os.getenv("HOME")
local prefix = home.."/pkgit"
install_directories = {
	bin			= prefix.."/bin",
	include	= prefix.."/include",
	lib			= prefix.."/lib",
	src			= prefix.."/src",
}

repositories = {
	pkgit = { "https://git.symlinx.net/pkgit" },
	beaker = {
		"https://git.symlinx.net/pkgit",
		dependencies = {}
		build = function()
			os.execute("make")
		end,
		pre_install = function() end
		install = function()
			os.execute("make install INSTALL_PREFIX="..prefix)
		end,
		post_install = function() end
	},
}

build_systems = {
	Makefile = {
		build = function()
			os.execute("make")
		end,
		install = function()
			os.execute("make install")
		end,
	},
}
