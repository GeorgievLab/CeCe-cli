
# CeCe CLI application

CeCe is a platform for advanced simulation of cells and cell signalling in 2D.

This repository contains only CLI application, for whole project see [https://github.com/GeorgievLab/CeCe](https://github.com/GeorgievLab/CeCe)

## Building from source

Building from source code is done by CMake (see Requirements).

### Linux & OSX

```bash
$ mkdir build && cd build
$ cmake ..
$ cmake --build .
```

### Windows
```batch
md build
cd build
cmake -G "Visual Studio 14 2015" ..
cmake --build .
```

### Requirements

* C++11 compiler - GCC, Clang, MSVC.
* [CMake](https://cmake.org) v2.8 or newer.
* [Boost C++ Libraries](http://www.boost.org) - Filesystem, System, Any, Tribool and Variant.
* [libpng](http://libpng.org/pub/png/libpng.html) - image capturing.

### Optional requirements

For some functionality and plugins additional libraries are required.

* [Python 2.7](http://python.org) - Python plugins.
* [zlib](http://zlib.net) - required by libpng.
* [GLFW3](http://www.glfw.org) - Simulation visualization.
