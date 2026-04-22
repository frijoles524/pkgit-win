build_systems["Makefile"] = function(name)
  os.execute("make")
end

build_systems["meson.build"] = function(name)
  os.execute("meson setup build && meson compile -C build")
end

build_systems["CMakeLists.txt"] = function(name)
  os.execute("mkdir build && cd build && cmake ..")
end

build_systems["Cargo.toml"] = function(name)
  os.execute("cargo build --release")
end

build_systems["v.mod"] = function(name)
  os.execute("v . -o "..name)
end

build_systems["build.zig"] = function(name)
  os.execute("zig build --release=fast")
end

build_systems["go.mod"] = function(name)
	os.execute("go build")
end

build_systems["build.ninja"] = function(name)
	os.execute("ninja -C build")
end

build_systems["configure"] = function(name)
	os.execute("autoreconf -if && ./configure")
end
