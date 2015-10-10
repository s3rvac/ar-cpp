ar-cpp
======

A C++ library for parsing of [ar](https://en.wikipedia.org/wiki/Ar_(Unix)
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

The library is in an **early stage of development.** It currently supports
basic extraction of GNU/System V archives.

Requirements
------------

To build the library, you need:
* A compiler supporting C++14, such as [GCC](https://gcc.gnu.org/) version >=
  4.9 or [Clang](http://clang.llvm.org/) version >= 3.4.
* [CMake](https://cmake.org/) version >= 2.8.

The library is developed and tested on Linux and Microsoft Windows (via
[MSYS2](https://sourceforge.net/projects/msys2/)).

Build and Installation
----------------------

* Clone the repository or download the sources into a directory. Lets call that
  directory `ar-cpp`.
* `cd ar-cpp`
* `mkdir build && cd build`
* `cmake ..`
* `make && make install`

You can pass additional parameters to `cmake`:
* `-DAR_DOC=1` to build with API documentation (requires
    [Doxygen](http://www.doxygen.org/), disabled by default).
* `-DAR_TOOLS=1` to build with tools (disabled by default).
* `-DAR_TESTS=1` to build with tests (disabled by default).
* `-DAR_COVERAGE=1` to build with code coverage support (requires
    [LCOV](http://ltp.sourceforge.net/coverage/lcov.php), disabled by default).
* `-DCMAKE_BUILD_TYPE=debug` to build with debugging information, which is
    useful during development. By default, the library is built in the
    `release` mode.
* `-DCMAKE_INSTALL_PREFIX:PATH=/usr` to set a custom installation path.
* `-G` to set a custom project files generator (the default one generates
  UNIX Makefiles). For example, for
  [MSYS2](https://sourceforge.net/projects/msys2/) on Windows, use `-G'MSYS
  Makefiles'`.

The `make` call supports standard parameters, such as:
* `-j N` to build the library by using `N` processors.
* `VERBOSE=1` to show verbose output when building the library.

Use
---

If you use [CMake](https://cmake.org/), you can incorporate the library into
your project in the following way:
```
set(ar_DIR "/path/to/installed/ar-cpp/lib/cmake")
find_package(ar)
include_directories(SYSTEM ${ar_INCLUDE_DIR})

add_executable(your_app your_app.cpp)
target_link_libraries(your_app ar)
```

API Documentation
-----------------

The latest API documentation is available
[here](http://projects.petrzemek.net/ar-cpp/doc/latest/).

License
-------

Copyright (c) 2015 Petr Zemek (<s3rvac@gmail.com>) and contributors.

Distributed under the MIT license. See the
[`LICENSE`](https://github.com/s3rvac/ar-cpp/blob/master/LICENSE) file for more
details.
