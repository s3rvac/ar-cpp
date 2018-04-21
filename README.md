ar-cpp
======

**Warning: This project was an experiment and is no longer maintained. I
suggest using a different library.**

A C++ library for parsing of [ar](https://en.wikipedia.org/wiki/Ar_(Unix))
archives.

``` cpp
#include <ar/ar.h>

auto files = ar::extract(ar::File::fromFilesystem("/path/to/archive.a"));
for (auto& file : files) {
    std::cout << file->getName() << "\n";
    file->saveCopyTo("/path/to/directory");
}
```

Status
------

The library currently supports basic extraction of GNU/System V archives.

Requirements
------------

To build the library, you need:
* A compiler supporting C++14, such as [GCC](https://gcc.gnu.org/) version >=
  4.9, [Clang](http://clang.llvm.org/) version >= 3.4, or Visual
  Studio 2015 or greater.
* [CMake](https://cmake.org/) version >= 3.5.

The library is developed and tested on Linux, although it should also work on
Windows (Visual Studio 2015 or greater), and possibly on macOS as well. If not,
please, [submit an issue](https://github.com/s3rvac/ar-cpp/issues).

Build and Installation
----------------------

* Clone the repository or download the sources into a directory. Let's call that
  directory `ar-cpp`.
* `cd ar-cpp`
* `mkdir build && cd build`
* `cmake ..`
* Linux: `make` and `make install`
* Windows: Open the generated `ar.sln` project file in Visual Studio and build it
  from there.

You can pass additional parameters to `cmake`:
* `-DAR_DOC=ON` to build with API documentation (requires
  [Doxygen](http://www.doxygen.org/), disabled by default).
* `-DAR_TOOLS=ON` to build with tools (disabled by default).
* `-DAR_TESTS=ON` to build with tests (requires
  [GoogleTest](https://github.com/google/googletest), disabled by default).
* `-DAR_COVERAGE=ON` to build with code coverage support (requires GCC and
  [LCOV](http://ltp.sourceforge.net/coverage/lcov.php), disabled by default).
* `-DCMAKE_BUILD_TYPE=Debug` to build with debugging information, which is
  useful during development. By default, the library is built in the `Release`
  mode.
* `-DCMAKE_INSTALL_PREFIX:PATH=ar-install-dir` to set a custom installation
  path.
* `-G` to set a custom project files generator (the default one generates UNIX
  Makefiles). For example, for Visual Studio 2015 on 64b Windows, use
  `-G"Visual Studio 14 2015 Win64"`.

The `make` call supports standard parameters, such as:
* `-j N` to build the library by using `N` processors.
* `VERBOSE=1` to show verbose output when building the library.

Use
---

After the installation, you can incorporate the library into your project in
the following way (provided that you use [CMake](https://cmake.org/)):
```
find_package(ar)

add_executable(your_app your_app.cpp)
target_link_libraries(your_app ar)
```
If you have not installed ar-cpp into your system (i.e. you installed it only
locally into some directory), you will have to provide the path to the
`$INSTALL_DIR/lib/cmake/ar` directory prior to calling `find_package(ar)`,
either by
```
set(ar_DIR "$INSTALL_DIR/lib/cmake/ar")
```
or via the following `cmake` parameter:
```
-Dar_DIR "$INSTALL_DIR/lib/cmake/ar"
```

API Documentation
-----------------

The latest API documentation is available
[here](https://projects.petrzemek.net/ar-cpp/doc/latest/).

Code Coverage
-------------

The latest code coverage by tests is available
[here](https://projects.petrzemek.net/ar-cpp/coverage/latest/).

License
-------

Copyright (c) 2015 Petr Zemek (<s3rvac@gmail.com>) and contributors.

Distributed under the MIT license. See the
[`LICENSE`](https://github.com/s3rvac/ar-cpp/blob/master/LICENSE) file for more
details.
