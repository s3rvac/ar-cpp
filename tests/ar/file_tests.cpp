///
/// @file      ar/file_tests.cpp
/// @copyright (c) 2015 by Petr Zemek (s3rvac@gmail.com) and contributors
/// @license   MIT, see the @c LICENSE file for more details
/// @brief     Tests for the @c file module.
///

#include <gtest/gtest.h>

#include "ar/file.h"
#include "ar/internal/utilities/os.h"

namespace ar {
namespace tests {

///
/// Tests for File.
///
class FileTests: public testing::Test {};

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

///
/// Tests for Files.
///
class FilesTests: public testing::Test {
protected:
	std::unique_ptr<File> anyFile();
	std::unique_ptr<File> fileNamed(const std::string& name);
};

std::unique_ptr<File> FilesTests::anyFile() {
	return fileNamed("noname");
}

std::unique_ptr<File> FilesTests::fileNamed(const std::string& name) {
	return File::fromContentWithName("content", name);
}

TEST_F(FilesTests,
ContainerIsEmptyAfterCreationByDefault) {
	Files files;

	ASSERT_TRUE(files.empty());
}

TEST_F(FilesTests,
ContainersSizeIsZeroAfterCreationByDefault) {
	Files files;

	ASSERT_EQ(0, files.size());
}

TEST_F(FilesTests,
ContainerIsNoLongerEmptyAfterAppendingFileIntoEmptyContainer) {
	Files files;

	files.push_back(anyFile());

	ASSERT_FALSE(files.empty());
}

TEST_F(FilesTests,
FrontReturnsReferenceToFirstFile) {
	Files files;
	files.push_back(fileNamed("a"));
	files.push_back(fileNamed("b"));

	ASSERT_EQ(files.front(), files.front());
	ASSERT_EQ("a", files.front()->getName());
}

TEST_F(FilesTests,
BackReturnsReferenceToLastFile) {
	Files files;
	files.push_back(fileNamed("a"));
	files.push_back(fileNamed("b"));

	ASSERT_EQ(files.back(), files.back());
	ASSERT_EQ("b", files.back()->getName());
}

TEST_F(FilesTests,
IterationOverNonConstContainerThroughBeginEndWorksCorrectly) {
	Files files;
	files.push_back(fileNamed("a"));
	files.push_back(fileNamed("b"));

	auto it = files.begin();
	ASSERT_EQ("a", (*it)->getName());
	++it;
	ASSERT_EQ("b", (*it)->getName());
	++it;
	ASSERT_EQ(it, files.end());
}

} // namespace tests
} // namespace ar
