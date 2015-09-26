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

namespace ar {
namespace internal {
namespace tests {

///
/// Base class for Extractor tests.
///
class BaseExtractorTests: public testing::Test {
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
/// Tests for extraction of GNU archives.
///
class GNUArchiveTests: public BaseExtractorTests {};

TEST_F(GNUArchiveTests,
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

TEST_F(GNUArchiveTests,
ExtractReturnsSingletonContainerForArchiveWithLookupTableAndSingleFile) {
	auto files = extractArchiveWithContent(
		"!<arch>\n"s +
		"/               0           0     0     0       14        `\n"s +
		"\x00\x00\x00\x10\x00\x00\x00\x52""func1\x00"s +
		"mod1.o/         0           0     0     644     18        `\n"s +
		"contents of mod1.o"s
	);

	ASSERT_EQ(1, files.size());
	auto& file = files.front();
	ASSERT_EQ("mod1.o", file->getName());
	ASSERT_EQ("contents of mod1.o", file->getContent());
}

TEST_F(GNUArchiveTests,
ExtractReturnsSingletonContainerForArchiveWithFileNameTableAndSingleFile) {
	auto files = extractArchiveWithContent(
		"!<arch>\n"s +
		"//                                              42        `\n"s +
		"very_long_name_of_a_module_in_archive.o/\n"s +
		"\n"
		"/0              0           0     0     644     22        `\n"s +
		"contents of the module"s
	);

	ASSERT_EQ(1, files.size());
	auto& file = files.front();
	ASSERT_EQ("very_long_name_of_a_module_in_archive.o", file->getName());
	ASSERT_EQ("contents of the module", file->getContent());
}

TEST_F(GNUArchiveTests,
ExtractThrowsInvalidArchiveErrorWhenFileNameIsNotEndedWithSlash) {
	ASSERT_THROW(
		extractArchiveWithContent(
			"!<arch>\n"s +
			"test.txt"s
		),
		InvalidArchiveError
	);
}

TEST_F(GNUArchiveTests,
ExtractThrowsInvalidArchiveErrorWhenFileNameIsEmpty) {
	ASSERT_THROW(
		extractArchiveWithContent(
			"!<arch>\n"s +
			// The first '/' denotes a symbol table, the second '/' is a file
			// with an empty name.
			"/               0           0     0     644     0         `\n"s +
			"/               0           0     0     644     0         `\n"s
		),
		InvalidArchiveError
	);
}

TEST_F(GNUArchiveTests,
ExtractThrowsInvalidArchiveErrorWhenFileSizeIsMissing) {
	ASSERT_THROW(
		extractArchiveWithContent(
			"!<arch>\n"s +
			"test.txt/       0           0     0     644"s
		),
		InvalidArchiveError
	);
}

TEST_F(GNUArchiveTests,
ExtractThrowsInvalidArchiveErrorWhenFileHeaderEndIsMissing) {
	ASSERT_THROW(
		extractArchiveWithContent(
			"!<arch>\n"s +
			"test.txt/       0           0     0     644     20\n"s
		),
		InvalidArchiveError
	);
}

TEST_F(GNUArchiveTests,
ExtractThrowsInvalidArchiveErrorWhenReadFileContentSizeIsLessThanSpecifiedFileSize) {
	ASSERT_THROW(
		extractArchiveWithContent(
			"!<arch>\n"s +
			"test.txt/       0           0     0     644     9999  `\n"s +
			"..."s
		),
		InvalidArchiveError
	);
}

TEST_F(GNUArchiveTests,
ExtractThrowsInvalidArchiveErrorWhenFileNameTableSizeIsMissing) {
	ASSERT_THROW(
		extractArchiveWithContent(
			"!<arch>\n"s +
			"//"s
		),
		InvalidArchiveError
	);
}

TEST_F(GNUArchiveTests,
ExtractThrowsInvalidArchiveErrorWhenHeaderEndOfFileNameTableIsInvalid) {
	ASSERT_THROW(
		extractArchiveWithContent(
			"!<arch>\n"s +
			"//                                              XXX"s
		),
		InvalidArchiveError
	);
}

TEST_F(GNUArchiveTests,
ExtractThrowsInvalidArchiveErrorWhenHeaderEndOfFileNameTableIsMissing) {
	ASSERT_THROW(
		extractArchiveWithContent(
			"!<arch>\n"s +
			"//                                              42"s
		),
		InvalidArchiveError
	);
}

TEST_F(GNUArchiveTests,
ExtractThrowsInvalidArchiveErrorWhenFileNameTableEndsPrematurely) {
	ASSERT_THROW(
		extractArchiveWithContent(
			"!<arch>\n"s +
			"//                                              42        `\n"s
		),
		InvalidArchiveError
	);
}

TEST_F(GNUArchiveTests,
ExtractThrowsInvalidArchiveErrorWhenFileNameInFileNameTableIsEmpty) {
	ASSERT_THROW(
		extractArchiveWithContent(
			"!<arch>\n"s +
			"//                                              2         `\n"s +
			"/\n"s
		),
		InvalidArchiveError
	);
}

} // namespace tests
} // namespace internal
} // namespace ar
