##
## Project:   ar-cpp
## Copyright: (c) 2015 by Petr Zemek <s3rvac@gmail.com>
## License:   MIT, see the LICENSE file for more details
##
## The project's main CMake configuration file.
##

cmake_minimum_required(VERSION 3.5)

project(ar CXX)

##
## Options.
##

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	message(STATUS "Setting build type to 'Release' as none was specified")
	set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type." FORCE)
endif()

option(AR_DOC "Build public API documentation (requires Doxygen)." OFF)
option(AR_INTERNAL_DOC "Build also the internal parts of the API documentation." OFF)
option(AR_TOOLS "Build tools." OFF)
option(AR_COVERAGE "Build with code coverage support (requires lcov and build with tests)." OFF)
option(AR_TESTS "Build tests." OFF)

if(AR_INTERNAL_DOC)
	set(AR_DOC ON)
endif()

if(AR_COVERAGE)
	set(AR_TESTS ON)
endif()

##
## Paths.
##

# Use GNUInstallDirs to install files into correct locations on all platforms.
include(GNUInstallDirs)

##
## Dependencies.
##

if(AR_TESTS)
	find_package(GTest REQUIRED)
endif()

##
## Global compiler options.
##

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
	# Standard warnings.
	add_compile_options(/W3)
else()
	# Issue all the warnings demanded by strict ISO C++.
	add_compile_options(-pedantic)

	# Abort compilation upon the first error.
	add_compile_options(-Wfatal-errors)

	# Standard warnings.
	add_compile_options(-Wall -Wextra)
endif()

##
## Code coverage.
##

if(AR_COVERAGE)
	# Enable code coverage.
	add_compile_options(--coverage)

	# Build with debugging information to make the output meaningful.
	add_compile_options(-g)

	# Disable optimizations to get the most accurate results.
	add_compile_options(-O0)

	# Add a custom target to generate the code coverage.
	add_custom_target(coverage
		COMMENT "Running the tests and generating code coverage"
		COMMAND "${PROJECT_BINARY_DIR}/tests/ar/ar-tests"
		COMMAND mkdir -p coverage
		COMMAND lcov --capture --no-external
			--directory include --directory "${PROJECT_BINARY_DIR}/include"
			--directory src --directory "${PROJECT_BINARY_DIR}/src"
			# Comment out the next line if you want to omit test code from the
			# coverage.
			--directory tests --directory "${PROJECT_BINARY_DIR}/tests"
			--output-file coverage/coverage.info
		COMMAND genhtml --show-details --num-spaces 4 --demangle-cpp
			--legend --title "ar-cpp code coverage"
			coverage/coverage.info --output-directory coverage
		WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")
endif()

##
## Subdirectories.
##

if(AR_DOC)
	add_subdirectory(doc)
endif()
add_subdirectory(include)
add_subdirectory(src)
if(AR_TESTS)
	add_subdirectory(tests)
endif()
