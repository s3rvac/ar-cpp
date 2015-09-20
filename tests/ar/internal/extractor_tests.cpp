///
/// @file      ar/internal/extractor_tests.cpp
/// @copyright (c) 2015 by Petr Zemek (s3rvac@gmail.com) and contributors
/// @license   MIT, see the @c LICENSE file for more details
/// @brief     Tests for the @c extractor module.
///

#include <gtest/gtest.h>

#include "ar/exceptions.h"
#include "ar/extraction.h"
#include "ar/file.h"
#include "ar/internal/extractor.h"

using namespace std::literals::string_literals;
using namespace testing;

namespace ar {
namespace internal {
namespace tests {

///
/// Base class for Extractor tests.
///
class BaseExtractorTests: public Test {
protected:
	static Files extractArchiveWithContent(
		const std::string& content);
};

///
/// A helper method to make the extraction more readable in tests.
///
Files BaseExtractorTests::extractArchiveWithContent(
		const std::string& content) {
	Extractor extractor;
	return extractor.extract(content);
}

///
/// Common extraction tests for all formats.
///
class CommonExtractionTests: public BaseExtractorTests {};

TEST_F(CommonExtractionTests,
ExtractReturnsEmptyContainerForEmptyArchive) {
	auto files = extractArchiveWithContent(
		"!<arch>\n"s
	);

	ASSERT_TRUE(files.empty());
}

TEST_F(CommonExtractionTests,
ExtractThrowsInvalidArchiveErrorWhenMagicStringIsNotPresent) {
	ASSERT_THROW(
		extractArchiveWithContent(""),
		InvalidArchiveError
	);
}

///
/// Tests for extraction of GNU archives without a lookup table.
///
class GNUArchiveWithoutLookupTableTests: public BaseExtractorTests {};

TEST_F(GNUArchiveWithoutLookupTableTests,
ExtractThrowsInvalidArchiveErrorWhenFileNameIsNotEndedWithSlash) {
	ASSERT_THROW(
		extractArchiveWithContent(
			"!<arch>\n"s +
			"test.txt"s
		),
		InvalidArchiveError
	);
}

TEST_F(GNUArchiveWithoutLookupTableTests,
ExtractThrowsInvalidArchiveErrorWhenFileHeaderEndIsMissing) {
	ASSERT_THROW(
		extractArchiveWithContent(
			"!<arch>\n"s +
			"test.txt/       0           0     0     644     20\n"s
		),
		InvalidArchiveError
	);
}

TEST_F(GNUArchiveWithoutLookupTableTests,
ExtractReturnsSingletonContainerForArchiveWithSingleFile) {
	auto files = extractArchiveWithContent(
		"!<arch>\n"s +
		"test.txt/       0           0     0     644     20        `\n"s +
		"contents of test.txt"s
	);

	ASSERT_EQ(1, files.size());
	auto& file = files.front();
	ASSERT_EQ("test.txt", file->getName());
	ASSERT_EQ("contents of test.txt", file->getContent());
}

} // namespace tests
} // namespace internal
} // namespace ar
