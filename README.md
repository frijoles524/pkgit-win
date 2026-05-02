<div align="center">

<h1>pkgit</h1>
(package it!)

</div>

# What is this?
pkgit is an unconventional package manager designed to compile & install packages directly from their git repository.

> ***[DISCLAIMER]***
>
> Due to the nature of pkgit, you are solely responsible for vetting the repos that you add to your system.
>
> Use at your own risk.

# Compile pkgit
Enter the project directory, and follow one of the following procedures.

## Using Make
```
make
```

## Using pkgit
```
pkgit build
```
Both methods will create an executable in the root directory of the project.

# Install pkgit
After compiling, run the following with root privilages:
```
make install
```

## Don't have root?
You can specify any install location with `PREFIX=<path>`:
```
make install PREFIX="/path/to/install"
```

# Usage
## Installing Packages
### Basic install
Assuming you have already added its respective repo, you can install a package by specifying its name:
```
pkgit install [pkg_name]
```
Or you can use the short command:
```
pkgit i [pkg_name]
```

### Repo install
If you haven't added the package's repository yet, or you just want to be specific, you can install the package using its git URL:
```
pkgit install [url.git]
```

## Removal
### Packages
Removing (uninstalling) a package is as simple as it seems:
```
pkgit remove [pkg_name]
```
Or the short command:
```
pkgit r [pkg_name]
```

## Dependency Management
As it is, pkgit is capable of dependency management, but you will likely have to specify the dependency URLs for each package you install in `<config_directory>/pkgit/init.lua`. There's not a universal way to check for dependencies without using an existing package manager (unless the package you're installing has a bldit.lua).

### Configuring pkgit
Thankfully, this is a very simple process.

To configure pkgit, you have one centralized configuration file: `<config_directory>/pkgit/init.lua`
Thanks to liblua, pkgit pushes a package.path variable directly to your configuration file.
This means that you can require any sub-file/directory, without wrestling with `require()`.

The overall structure of the configuration file looks like this:
```lua
--[[
  - install directories -
  this is where every package you
  get with pkgit is installed,
  with respect to each of it's
  files' types:

  install_directories = {
    bin     = "...",
    include = "...",
    lib     = "...",
    src     = "...",
  }

  it is recommended that you create
  a prefix variable to prepend to
  these directories, like so:
]]
local prefix = os.getenv("HOME").."/.local" -- for user-level installs
install_directories = {
  bin     = prefix.."/bin",
  include = prefix.."/include",
  lib     = prefix.."/lib",
  src     = prefix.."/src",
}

--[[
  - your repositories -
  this is where you store your
  own custom urls, dependencies,
  and functions to install
  whatever programs you desire.
]]
repositories = {
  example_name = {
    url = "https://...",
    dependencies = {
      ...
    },
    build = function() ... end
    pre_install = function() ... end
    install = function() ... end
    post_install = function() ... end
  }
}
--[[
  pkgit also creates a 'repos.lua' file in
  your config directory,  which you can
  require here to automatically add repos
  that you install from.
]]
require('repos')

--[[
  - standard build systems -
  will be auto-detected if required
  functions aren't in 'repositories'
  or a repo's 'bldit.lua'
]]
build_systems = {
  ["filename.extension"] = {
    build = function() ... end
    pre_install = function() ... end
    install = function() ... end
    post_install = function() ... end
  }
}
```

For each dependency, all you need to do is paste the dependency's remote git URL in its own url variable.

Here's an example for mush:
```lua
repositories = {
  mush = {
    url = "https://github.com/dacctal/mush",
    dependencies = {
      mpv = { url = "https://github.com/mpv-player/mpv" },

      ["yt-dlp"] = { url = "https://github.com/yt-dlp/yt-dlp" },
      --[[
        for dependencies with names that contain illegal
        characters in lua, you'll want to wrap them in
        [""] to avoid problems.
      ]]

      ffmpeg = { url = "https://github.com/FFmpeg/FFmpeg" },
      curl = { url = "https://github.com/curl/curl" },
      mutagen = { url = "https://github.com/quodlibet/mutagen" }
    }
  }
}
```

That's it! pkgit will read from this file and resolve these
dependencies automatically.

### [DEVELOPER]: bldit.lua
If you want your own package's dependencies, version, compilation,
and other aspects to be properly resolved in pkgit, you can create a
`bldit.lua` file in the root directory of your project's git repo.

Do not name it anything other than `bldit.lua`, otherwise pkgit
will not find the file.

> [!WARNING]
> Recursive dependency management does NOT work in pkgit, so you may want to list your dependencies accordingly.

Example `bldit.lua` that works for pkgit:
```lua
bldit_version = "0.0.0"
package_version = "0.0.0"
--[[
  versions have to be strings, 
  because some devs like to
  have fun and whimsical
  version numbers :D
]]

global_dependencies = {}
-- dependencies for all targets

targets = {
	default = {
		dependencies = {},
		-- target-specific dependencies
		build = function(name)
			os.execute("make")
		end,
		pre_install = function() end,
		install = function(prefix)
			os.execute("make install")
		end,
		post_install = function() end,
	}
	--[[
    could also have targets for 'client'
    or 'server.' useful for monorepos
    and related use-cases.
	]]
}
```
