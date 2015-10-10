##
## Project:   ar-cpp
## Copyright: (c) 2015 by Petr Zemek <s3rvac@gmail.com> and contributors
## License:   MIT, see the LICENSE file for more details
##
## CMake configuration file for tests.
##

if(NOT AR_TESTS)
	return()
endif()

##
## Dependencies.
##

# Google Test
find_package(GTest QUIET)
if(NOT GTEST_FOUND)
	ExternalProject_Add(googletest
		URL http://googletest.googlecode.com/files/gtest-1.7.0.zip
		# Disable the install step.
		INSTALL_COMMAND ""
		# Wrap the download, configure and build steps in a script to log the
		# output.
		LOG_DOWNLOAD ON
		LOG_CONFIGURE ON
		LOG_BUILD ON
	)
	ExternalProject_Get_Property(googletest source_dir)
	ExternalProject_Get_Property(googletest binary_dir)
	set(GTEST_INCLUDE_DIR "${source_dir}/include")
	set(GTEST_LIBRARY_DIR "${binary_dir}")
	set(GTEST_LIBRARY "gtest")
	set(GTEST_MAIN_LIBRARY "gtest_main")
	link_directories(${GTEST_LIBRARY_DIR})
endif()
include_directories(SYSTEM ${GTEST_INCLUDE_DIR})

##
## Includes.
##

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

##
## Subdirectories.
##

add_subdirectory(ar)
