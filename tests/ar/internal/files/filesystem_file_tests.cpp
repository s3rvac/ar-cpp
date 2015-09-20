///
/// @file      ar/internal/files/filesystem_file_tests.cpp
/// @copyright (c) 2015 by Petr Zemek (s3rvac@gmail.com) and contributors
/// @license   MIT, see the @c LICENSE file for more details
/// @brief     Tests for the @c filesystem_file module.
///

#include <gtest/gtest.h>

#include "ar/internal/files/filesystem_file.h"
#include "ar/internal/utilities/os.h"
#include "ar/test_utilities/tmp_file.h"

using namespace ar::tests;

namespace ar {
namespace internal {
namespace tests {

///
/// Tests for FilesystemFile.
///
class FilesystemFileTests: public testing::Test {};

TEST_F(FilesystemFileTests,
GetNameReturnsCorrectValueWhenNoCustomNameIsGiven) {
#ifdef AR_OS_WINDOWS
	FilesystemFile file{R"(C:\\/path/to/file.txt)"};
#else
	FilesystemFile file{"/path/to/file.txt"};
#endif

	ASSERT_EQ("file.txt", file.getName());
}

TEST_F(FilesystemFileTests,
GetNameReturnsCorrectValueWhenCustomNameIsGiven) {
	FilesystemFile file{"/path/to/file.txt", "another_file.txt"};

	ASSERT_EQ("another_file.txt", file.getName());
}

TEST_F(FilesystemFileTests,
GetContentReturnsCorrectContent) {
	auto tmpFile = TmpFile::createWithContent("content");
	FilesystemFile file{tmpFile->getPath()};

	ASSERT_EQ("content", file.getContent());
}

} // namespace tests
} // namespace internal
} // namespace ar
