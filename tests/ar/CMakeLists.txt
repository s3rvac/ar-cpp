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

add_executable(ar_tests ${AR_TESTS_SOURCES})
if(NOT GTEST_FOUND)
	add_dependencies(ar googletest)
endif()
if(NOT GTEST_FOUND)
	add_dependencies(ar googlemock)
endif()
target_link_libraries(ar_tests
	ar
	${GTEST_LIBRARY}
	${GTEST_MAIN_LIBRARY}
)

install(TARGETS ar_tests DESTINATION "${INSTALL_BIN_DIR}/tests")
