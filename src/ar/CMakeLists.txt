##
## Project:   ar-cpp
## Copyright: (c) 2015 by Petr Zemek <s3rvac@gmail.com> and contributors
## License:   MIT, see the LICENSE file for more details
##
## CMake configuration file for the library source files.
##

set(AR_SOURCES
	exceptions.cpp
	extraction.cpp
	file.cpp
	internal/extractor.cpp
	internal/files/filesystem_file.cpp
	internal/files/string_file.cpp
	internal/utilities/os.cpp
)

add_library(ar ${AR_SOURCES})

install(TARGETS ar DESTINATION "${INSTALL_LIB_DIR}" EXPORT ar)
install(EXPORT ar DESTINATION "${INSTALL_LIB_CMAKE_DIR}")

# Configuration file for find_package(ar).
configure_file(
	ar-config.cmake.in
	"${CMAKE_BINARY_DIR}/ar-config-install.cmake"
	@ONLY
)
install(
	FILES "${CMAKE_BINARY_DIR}/ar-config-install.cmake"
	DESTINATION "${INSTALL_LIB_CMAKE_DIR}"
	RENAME ar-config.cmake
)
