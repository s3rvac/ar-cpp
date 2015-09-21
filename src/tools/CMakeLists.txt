##
## Project:   ar-cpp
## Copyright: (c) 2015 by Petr Zemek <s3rvac@gmail.com> and contributors
## License:   MIT, see the LICENSE file for more details
##
## CMake configuration file for the tools.
##

if(NOT AR_TOOLS)
	return()
endif()

# ar-extract
add_executable(ar-extract ar-extract.cpp)
target_link_libraries(ar-extract ar)
install(TARGETS ar-extract DESTINATION "${INSTALL_BIN_DIR}")

# ar-info
add_executable(ar-info ar-info.cpp)
target_link_libraries(ar-info ar)
install(TARGETS ar-info DESTINATION "${INSTALL_BIN_DIR}")
