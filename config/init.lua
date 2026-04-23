-- initializing the path for configuration
-- DO NOT CHANGE
home_dir = os.getenv("HOME")
package.path = string.format("%s;%s/.config/pkgit/?.lua", package.path, home_dir)
-- require other lua scripts
require "build_systems"
require "repositories"
require "install_directories"
