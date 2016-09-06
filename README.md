
# CeCe CLI application

[![Build Status](https://travis-ci.org/GeorgievLab/CeCe-cli.svg?branch=master)](https://travis-ci.org/GeorgievLab/CeCe-cli)
[![Build status](https://ci.appveyor.com/api/projects/status/slowt8psw5qpgj3v/branch/master?svg=true)](https://ci.appveyor.com/project/NTSFka/cece-cli/branch/master)

CeCe is a platform for advanced simulation of cells and cell signalling in 2D.

This repository contains only CLI application, for whole project see [https://github.com/GeorgievLab/CeCe](https://github.com/GeorgievLab/CeCe)

## Building from source

Building from source code is done by CMake (see Requirements).

### Linux & macOS

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
* [CMake](https://cmake.org) v3.1 or newer.
