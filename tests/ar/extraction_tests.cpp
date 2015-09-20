///
/// @file      ar/extraction_tests.cpp
/// @copyright (c) 2015 by Petr Zemek (s3rvac@gmail.com) and contributors
/// @license   MIT, see the @c LICENSE file for more details
/// @brief     Tests for the @c extraction module.
///

#include <gtest/gtest.h>

#include "ar/exceptions.h"
#include "ar/extraction.h"
#include "ar/file.h"

namespace ar {
namespace tests {

///
/// Tests for extract().
///
class ExtractTests: public testing::Test {};

TEST_F(ExtractTests,
ExtractReturnsEmptyContainerForEmptyArchive) {
	auto files = extract(
		File::fromContentWithName("!<arch>\n", "archive.a")
	);

	ASSERT_TRUE(files.empty());
}

TEST_F(ExtractTests,
ExtractThrowsInvalidArchiveErrorWhenMagicStringIsNotPresent) {
	ASSERT_THROW(
		extract(File::fromContentWithName("", "archive.a")),
		InvalidArchiveError
	);
}

TEST_F(ExtractTests,
ExtractReturnsSingletonContainerForArchiveWithSingleFile) {
	auto files = extract(
		File::fromContentWithName(
			"!<arch>\n"
			"test.txt/       0           0     0     644     20        `\n"
			"contents of test.txt"
		,
			"archive.a"
		)
	);

	ASSERT_EQ(1, files.size());
	auto& file = files.front();
	ASSERT_EQ("test.txt", file->getName());
	ASSERT_EQ("contents of test.txt", file->getContent());
}

} // namespace tests
} // namespace ar
