
# CeCe CLI application

[![Build Status](https://travis-ci.org/GeorgievLab/CeCe-cli.svg?branch=master)](https://travis-ci.org/GeorgievLab/CeCe-cli)
[![Build status](https://ci.appveyor.com/api/projects/status/slowt8psw5qpgj3v/branch/master?svg=true)](https://ci.appveyor.com/project/NTSFka/cece-cli/branch/master)

[CeCe](http://georgievlab.github.io/CeCe/) is a platform for advanced simulation of cell communication in dynamic environment.

This repository contains only CLI application, for whole project see [https://github.com/GeorgievLab/CeCe](https://github.com/GeorgievLab/CeCe)

## CI builds

Prebuild packages of the lastest version on CI services (Travis-CI, Appveyor).

| Platform    | Lastest |
| ----------- | ------- |
| Windows x64 | [ ![Download](https://api.bintray.com/packages/georgievlab/CeCe-cli-ci/win64/images/download.svg) ](https://bintray.com/georgievlab/CeCe-cli-ci/win64/_latestVersion) |
| macOS/OSX   | [ ![Download](https://api.bintray.com/packages/georgievlab/CeCe-cli-ci/macOS/images/download.svg) ](https://bintray.com/georgievlab/CeCe-cli-ci/macOS/_latestVersion) |
| Linux x64   | [ ![Download](https://api.bintray.com/packages/georgievlab/CeCe-cli-ci/linux-x64/images/download.svg) ](https://bintray.com/georgievlab/CeCe-cli-ci/linux-x64/_latestVersion) |

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
