##
## Project:   ar-cpp
## Copyright: (c) 2015 by Petr Zemek <s3rvac@gmail.com> and contributors
## License:   MIT, see the LICENSE file for more details
##
## CMake configuration file for the source files.
##

add_subdirectory(ar)
if(AR_TOOLS)
	add_subdirectory(tools)
endif()
