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
## Subdirectories.
##

add_subdirectory(ar)
