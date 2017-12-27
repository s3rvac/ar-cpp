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
target_include_directories(ar
	PUBLIC
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/ar>
)
install(TARGETS ar EXPORT ar-target
	ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
	INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
	LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
	RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
)
install(EXPORT ar-target
	FILE ar-config.cmake
	DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/ar"
)
