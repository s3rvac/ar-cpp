ar-cpp
======

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

The library currently supports basic extraction of GNU/System V archives. More
features are on their way, depending on the needs of users.

Requirements
------------

To build the library, you need:
* A compiler supporting C++14, such as [GCC](https://gcc.gnu.org/) version >=
  4.9, [Clang](http://clang.llvm.org/) version >= 3.4, or Visual
  Studio 2015 or greater.
* [CMake](https://cmake.org/) version >= 3.0.

The library is developed and tested on Linux, although it should also work on
Windows (Visual Studio 2015 or greater).

Build and Installation
----------------------

* Clone the repository or download the sources into a directory. Lets call that
  directory `ar-cpp`.
* `cd ar-cpp`
* `mkdir build && cd build`
* `cmake ..`
* `make && make install`

You can pass additional parameters to `cmake`:
* `-DAR_DOC=ON` to build with API documentation (requires
    [Doxygen](http://www.doxygen.org/), disabled by default).
* `-DAR_TOOLS=ON` to build with tools (disabled by default).
* `-DAR_TESTS=ON` to build with tests (disabled by default).
* `-DAR_COVERAGE=ON` to build with code coverage support (requires
    [LCOV](http://ltp.sourceforge.net/coverage/lcov.php), disabled by default).
* `-DCMAKE_BUILD_TYPE=Debug` to build with debugging information, which is
    useful during development. By default, the library is built in the
    `Release` mode.
* `-DCMAKE_INSTALL_PREFIX:PATH=/usr` to set a custom installation path.
* `-G` to set a custom project files generator (the default one generates
  UNIX Makefiles). For example, for Visual Studio 2015 on 64b Windows, use
  `-G"Visual Studio 14 2015 Win64"`.

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
