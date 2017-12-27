##
## Project:   ar-cpp
## Copyright: (c) 2015 by Petr Zemek <s3rvac@gmail.com> and contributors
## License:   MIT, see the LICENSE file for more details
##
## CMake configuration file for the tests of the library.
##

set(AR_TESTS_SOURCES
	exceptions_tests.cpp
	extraction_tests.cpp
	file_tests.cpp
	internal/extractor_tests.cpp
	internal/files/filesystem_file_tests.cpp
	internal/files/string_file_tests.cpp
	internal/utilities/os_tests.cpp
	test_utilities/tmp_file.cpp
)

add_executable(ar-tests ${AR_TESTS_SOURCES})
target_include_directories(ar-tests PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../")
target_link_libraries(ar-tests PRIVATE
	ar
	GTest::GTest
	GTest::Main
)
install(TARGETS ar-tests DESTINATION "${CMAKE_INSTALL_BINDIR}")
