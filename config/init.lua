-- initializing the path for configuration
-- DO NOT CHANGE
home_dir = os.getenv("HOME")
package.path = package.path .. ";" .. home_dir .. "/.config/pkgit/?.lua"

-- require other lua scripts
require "build_systems"
require "repositories"
require "install_directories"
