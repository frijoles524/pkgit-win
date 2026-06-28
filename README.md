<div align="center">
<h1>pkgit</h1>
(package it!)
</div>

# What is this?
pkgit is an unconventional package manager designed to compile & install packages directly from their git repository.

# License
pkgit is licensed under the GNU-GPL-2.0-or-later.
if you did not get a copy, please see <https://www.gnu.org/licenses/>.

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
pkgit --build
```
Both methods will create an executable in the root directory of the project.

You'll probably also want to generate a base configuration file if you don't already have one.
Run this as a user to generate the config in ~/.config/pkgit:
```
make defconfig
```
You can also run this with root using sudo/doas to genereate the config in /etc/pkgit
```
sudo make defconfig
```

# Install pkgit
After compiling, run the following with root privilages:
```
make install
```

## Don't have root?
You can specify any install location with `PREFIX=<path>`. A popular option is `~/.local`:
```
make install PREFIX="~/.local"
```

# Usage
## Command Syntax
The structure in which you type commands into pkgit is very standard:
```
pkgit [--flag|-f] <package>
```

Flags have two types; long, and short.
The short type of each command uses the first letter of its long counterpart.
For example:
Long: `pkgit --install`
Short: `pkgit -i`

If you use the short version of commands,
you can chain them together into one argument:
```
pkgit -qif <package>
```

This example uses the `--quiet`, `--install`, and `--force` flags to:
  --quiet: minimize logs to stdout
  --install: install a package
  --force: even if already installed, will force the package to be installed

## Installing Packages
### Pre-installation
Before you use any programs that you installed with pkgit,
you need to specify the path of the binaries in your shell's configuration.

For most users, this is the command:
```sh
export PATH="$HOME/.local/bin:$PATH"
```
or for fish users:
```fish
fish_add_path $HOME/.local/bin
```
or for csh users:
```csh
setenv PATH $HOME/.local/bin:$PATH
```

### Basic install
Assuming you have already added its respective repo, you can install a package by specifying its name:
```
pkgit --install [pkg_name]
```
Or you can use the short command:
```
pkgit -i [pkg_name]
```

### Specific version install
You can specify a version of any package based on its tags with '@' separating the name from the version:
```
pkgit --install [pkg_name]@[version]
```

### Specific target install
You can specify a target of any package based on its configuration in bldit.lua or init.lua with ',' separating the name from the target:
```
pkgit --install [pkg_name],[target]
```

### Combined target and version install
You can specify both the target and the version of any package you install in the same command (order does not matter as long as the package name is first):
```
pkgit --install [pkg_name],[target]@[version]
```
```
pkgit --install [pkg_name]@[version],[target]
```

### Repo install
If you haven't added the package's repository yet, or you just want to be specific, you can install the package using its git URL:
```
pkgit --install [url.git]
```
This also works with the target and version syntax.

### Local install
If you want to install a package from a local code repository,
and want to take advantage of pkgit's build system autodetection,
you can enter that repository's root directory and install it from there:
```
pkgit --install .
```

## Building packages
*FOR DEVELOPERS*
You can also use pkgit as a sort of meta-build-system to automatically compile any supported project with the build command:
```
pkgit --build [/path/to/project]
```
This can also be done without specifying the path (`pkgit --build`) if you're in the project's root directory

## Removing Packages
Removing (uninstalling) a package is as simple as it seems:
```
pkgit --remove [pkg_name]
```
Or the short command:
```
pkgit -r [pkg_name]
```

## Updating Packages
You can easily update your installed packages by running:
```
pkgit --update
```
Or the short command:
```
pkgit -u
```

## Declaring Packages
If you prefer a declarative approach, you can use the config file (read below) as a package declaration file.
When you're ready, you can declare all your packages at once with:
```
pkgit --declare
```
Or the short command:
```
pkgit -d
```

## Dependency Management
As it is, pkgit is capable of dependency management, but you will likely have to specify the dependency URLs for each package you install in `<config_directory>/pkgit/init.lua`.
There's not a universal way to check for dependencies without using an existing package manager (unless the package you're installing has a bldit.lua).

## Configuring pkgit
Thankfully, this is a very simple process.

To configure pkgit, you have one centralized configuration file: `<config_directory>/pkgit/init.lua`
Thanks to liblua, pkgit pushes a package.path variable directly to your configuration file.
This means that you can require any sub-file/directory, without wrestling with `require()`.

The overall structure of the configuration file is explained in detail in `config/init.lua`

## [DEVELOPER]: bldit.lua
If you want your own package's dependencies, version, compilation,
and other aspects to be properly resolved in pkgit, you can create a
`bldit.lua` file in the root directory of your project's git repo.

Do not name it anything other than `bldit.lua`, otherwise pkgit
will not find the file.

A great example for `bldit.lua` is right here in the pkgit repository!
Check it out if you want to know how it's structured.

# Contributing to pkgit
If you want to contribute to pkgit, please refer to CONTRIBUTING.md
