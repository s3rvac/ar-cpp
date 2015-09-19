///
/// @file      ar/file_tests.cpp
/// @copyright (c) 2015 by Petr Zemek (s3rvac@gmail.com) and contributors
/// @license   MIT, see the @c LICENSE file for more details
/// @brief     Tests for the @c file module.
///

#include <gtest/gtest.h>

#include "ar/file.h"
#include "ar/internal/utilities/os.h"

using namespace testing;

namespace ar {
namespace tests {

///
/// Tests for File.
///
class FileTests: public Test {};

TEST_F(FileTests,
FromContentWithNameReturnsFileWithCorrectContentAndName) {
	auto file = File::fromContentWithName("content", "file.txt");

	ASSERT_EQ("content", file->getContent());
	ASSERT_EQ("file.txt", file->getName());
}

TEST_F(FileTests,
FromFilesystemReturnsFileWithCorrectName) {
#ifdef AR_OS_WINDOWS
	auto file = File::fromFilesystem(R"(C:\\/path/to/file.txt)");
#else
	auto file = File::fromFilesystem("/path/to/file.txt");
#endif

	ASSERT_EQ("file.txt", file->getName());
}

TEST_F(FileTests,
FromFilesystemWithOtherNameReturnsFileWithCorrectName) {
#ifdef AR_OS_WINDOWS
	auto file = File::fromFilesystemWithOtherName(
		R"(C:\\/path/to/file.txt)", "other.txt");
#else
	auto file = File::fromFilesystemWithOtherName(
		"/path/to/file.txt", "other.txt");
#endif

	ASSERT_EQ("other.txt", file->getName());
}

} // namespace tests
} // namespace ar
