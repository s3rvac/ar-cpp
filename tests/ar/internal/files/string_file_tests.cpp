///
/// @file      ar/internal/files/string_file_tests.cpp
/// @copyright (c) 2015 by Petr Zemek (s3rvac@gmail.com) and contributors
/// @license   MIT, see the @c LICENSE file for more details
/// @brief     Tests for the @c string_file module.
///

#include <gtest/gtest.h>

#include "ar/internal/files/string_file.h"
#include "ar/internal/utilities/os.h"
#include "ar/test_utilities/tmp_file.h"

using namespace ar::tests;

namespace ar {
namespace internal {
namespace tests {

///
/// Tests for StringFile.
///
class StringFileTests: public testing::Test {};

TEST_F(StringFileTests,
FileHasCorrectContentUponCreation) {
	StringFile file{"content"};

	ASSERT_EQ("content", file.getContent());
}

TEST_F(StringFileTests,
GetNameReturnsCorrectNameWhenFileHasName) {
	StringFile file{"content", "file.txt"};

	ASSERT_EQ("file.txt", file.getName());
}

TEST_F(StringFileTests,
GetNameReturnsEmptyStringWhenFileHasNoName) {
	StringFile file{"content"};

	ASSERT_EQ("", file.getName());
}

TEST_F(StringFileTests,
SaveCopyToSavesCopyOfFileToGivenDirectory) {
	const std::string Content{"content"};
	const std::string Name{"ar-stringfile-save-copy-to-test.txt"};
	StringFile file{Content, Name};

	file.saveCopyTo(".");

	RemoveFileOnDestruction remover{Name};
	ASSERT_EQ(Content, readFile(Name));
}

} // namespace tests
} // namespace internal
} // namespace ar
