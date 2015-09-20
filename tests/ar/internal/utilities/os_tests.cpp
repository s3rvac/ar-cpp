///
/// @file      ar/internal/utilities/os_tests.cpp
/// @copyright (c) 2015 by Petr Zemek (s3rvac@gmail.com) and contributors
/// @license   MIT, see the @c LICENSE file for more details
/// @brief     Tests for the @c os module.
///

#include <gtest/gtest.h>

#include "ar/exceptions.h"
#include "ar/internal/utilities/os.h"
#include "ar/test_utilities/tmp_file.h"

using namespace ar::tests;

namespace ar {
namespace internal {
namespace tests {

///
/// Tests for fileNameFromPath().
///
class FileNameFromPathTests: public testing::Test {};

TEST_F(FileNameFromPathTests,
FileNameFromPathReturnsCorrectNameForPathWithSeparators) {
#ifdef AR_OS_WINDOWS
	ASSERT_EQ("file.txt", fileNameFromPath(R"(C:\\path\to\file.txt)"));
	ASSERT_EQ("file.txt", fileNameFromPath(R"(C:/path/to/file.txt)"));
#else
	ASSERT_EQ("file.txt", fileNameFromPath("/path/to/file.txt"));
#endif
}

TEST_F(FileNameFromPathTests,
FileNameFromPathContainingJustFileNameReturnsCorrectName) {
	ASSERT_EQ("file.txt", fileNameFromPath("file.txt"));
}

TEST_F(FileNameFromPathTests,
FileNameFromPathContainingNoFileNameReturnsEmptyString) {
#ifdef AR_OS_WINDOWS
	ASSERT_EQ("", fileNameFromPath(R"(C:\\path\to\dir\)"));
	ASSERT_EQ("", fileNameFromPath(R"(C:/path/to/dir/)"));
#else
	ASSERT_EQ("", fileNameFromPath("/path/to/dir/"));
#endif
}

///
/// Tests for joinPaths().
///
class JoinPathsTests: public testing::Test {};

TEST_F(JoinPathsTests,
JoinPathsReturnsSecondPathWhenFirstPathIsEmpty) {
	ASSERT_EQ("dir", joinPaths("", "dir"));
}

TEST_F(JoinPathsTests,
JoinPathsReturnsFirstPathWhenSecondPathIsEmpty) {
	ASSERT_EQ("dir", joinPaths("dir", ""));
}

TEST_F(JoinPathsTests,
JoinPathsReturnsSecondPathWhenSecondPathIsAbsolute) {
#ifdef AR_OS_WINDOWS
	ASSERT_EQ(R"(C:\\path)", joinPaths("dir", R"(C:\\path)"));
	ASSERT_EQ(R"(C:/path)", joinPaths("dir", R"(C:/path)"));
#endif
	ASSERT_EQ("/path/to", joinPaths("dir", "/path/to"));
}

TEST_F(JoinPathsTests,
TwoPathsAreJoinedCorrectlyWhenFirstPathDoesNotEndWithSlash) {
#ifdef AR_OS_WINDOWS
	ASSERT_EQ(R"(C:\\path\to\dir)", joinPaths(R"(C:\\path\to)", "dir"));
	ASSERT_EQ(R"(C:/path/to/dir)", joinPaths(R"(C:/path/to)", "dir"));
#endif
	ASSERT_EQ("/path/to/dir", joinPaths("/path/to", "dir"));
}

TEST_F(JoinPathsTests,
TwoPathsAreJoinedCorrectlyWhenFirstPathEndsWithSlash) {
#ifdef AR_OS_WINDOWS
	ASSERT_EQ(R"(C:\\path\to\dir)", joinPaths(R"(C:\\path\to\)", "dir"));
	ASSERT_EQ(R"(C:/path/to/dir)", joinPaths(R"(C:/path/to/)", "dir"));
#endif
	ASSERT_EQ("/path/to/dir", joinPaths("/path/to/", "dir"));
}

///
/// Tests for readFile().
///
class ReadFileTests: public testing::Test {};

TEST_F(ReadFileTests,
ReturnsCorrectContentWhenFileExists) {
	auto tmpFile = TmpFile::createWithContent("content");

	ASSERT_EQ("content", readFile(tmpFile->getPath()));
}

TEST_F(ReadFileTests,
ThrowsIoErrorWhenFileDoesNotExist) {
	ASSERT_THROW(readFile("nonexisting-file"), IoError);
}

///
/// Tests for writeFile().
///
class WriteFileTests: public testing::Test {};

///
/// Tests for copyFile().
///
class CopyFileTests: public testing::Test {};

TEST_F(CopyFileTests,
ThrowsIoErrorWhenSourceFileDoesNotExist) {
	ASSERT_THROW(copyFile("nonexisting-file", "any-file"), IoError);
}

} // namespace tests
} // namespace internal
} // namespace ar
