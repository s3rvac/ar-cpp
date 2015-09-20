##
## Project:   ar-cpp
## Copyright: (c) 2015 by Petr Zemek <s3rvac@gmail.com>
## License:   MIT, see the LICENSE file for more details
##
## The project's main CMake configuration file.
##

cmake_minimum_required(VERSION 2.8)

project(ar-cpp CXX C)

##
## Options.
##

option(AR_DOC "Build public API documentation (requires Doxygen)." OFF)
option(AR_INTERNAL_DOC "Build also the internal parts of the API documentation." OFF)
option(AR_TOOLS "Build tools." OFF)
option(AR_COVERAGE "Build with code coverage support (requires lcov and build with tests)." OFF)
option(AR_TESTS "Build tests." OFF)

if(${AR_INTERNAL_DOC})
	set(AR_DOC ON)
endif()

if(${AR_COVERAGE})
	set(AR_TESTS ON)
endif()

##
## Paths.
##

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install" CACHE PATH "default install path" FORCE)
endif()
set(INSTALL_BIN_DIR "${CMAKE_INSTALL_PREFIX}/bin")
set(INSTALL_DOC_DIR "${CMAKE_INSTALL_PREFIX}/doc")
set(INSTALL_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include")
set(INSTALL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib")
set(INSTALL_LIB_CMAKE_DIR "${INSTALL_LIB_DIR}/cmake")

##
## Dependencies.
##

include(ExternalProject)

# The default root directory for external projects.
set_directory_properties(PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/third_party)

##
## Includes.
##

include_directories(include)

##
## Compiler options.
##

# Get the version of the used C++ compiler. It is needed to enable additional
# warnings because some of them are not present in all compilers/versions.
execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion
	OUTPUT_VARIABLE CXX_COMPILER_VERSION)

# C++14 is required.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pedantic")

# Abort compilation upon the first error.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wfatal-errors")

# Standard warning parameters.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# Additional warning parameters.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wcast-align")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wcast-qual")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wfloat-equal")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wold-style-cast")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wswitch-default")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wuninitialized")
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wzero-as-null-pointer-constant")
	if(CXX_COMPILER_VERSION VERSION_GREATER 4.8 OR CXX_COMPILER_VERSION VERSION_EQUAL 4.8)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wuseless-cast")
	endif()
	if(CXX_COMPILER_VERSION VERSION_GREATER 5 OR CXX_COMPILER_VERSION VERSION_EQUAL 5)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wsuggest-override")
	endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wconditional-uninitialized")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wheader-hygiene")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wnon-literal-null-conversion")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wreserved-id-macro")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wsometimes-uninitialized")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wunreachable-code")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wunused-exception-parameter")
endif()

##
## Code coverage.
##

if(AR_COVERAGE)
	# Enable code coverage.
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")

	# Build with debugging information to make the output meaningful.
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

	# Disable optimizations to get the most accurate results.
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

	# Add a custom target to generate the code coverage.
	add_custom_target(coverage
		COMMENT "Running the tests and generating code coverage"
		COMMAND "${PROJECT_BINARY_DIR}/tests/ar/ar_tests"
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

add_subdirectory(doc)
add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(tests)
