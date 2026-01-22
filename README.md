# Compile pkgit
Enter the project directory, and follow one of the following procedures.
## Using Muon
```
muon setup build
muon build build
```
## Using Meson
```
meson setup build
ninja -C build
```
Both methods will put the executable for pkgit into the `build` directory.
