///
/// @file      ar/test_utilities/tmp_file.cpp
/// @copyright (c) 2015 by Petr Zemek (s3rvac@gmail.com) and contributors
/// @license   MIT, see the @c LICENSE file for more details
/// @brief     Implementation of the temporary-file utilities.
///

#include <cstdio>
#include <fstream>
#include <random>

#include "ar/test_utilities/tmp_file.h"

namespace ar {
namespace tests {

namespace {

///
/// Returns a path to a unique, temporary file.
///
std::string getUniqueTemporaryFilePath() {
	// As the creation of a temporary file with accessible name in the standard
	// C++ is painful, simply create the temporary file in the current
	// directory. After all, this function is only used in tests.
	//
	// The use of static variables makes the code unsafe for threads, but since
	// we do not use threads in tests, it is OK. The problem is that at least
	// on Windows (MSYS2), std::random_device is only a pseudo-random number
	// generator that always generates the same sequence. Therefore, we
	// initialize the random number generator when the tests start (when
	// getUniqueTemporaryFilePath() is first called) and use it for all tests.
	static std::random_device rd;
	static std::mt19937 mt{rd()};
	static std::uniform_int_distribution<std::size_t> dist;
	return "ar-cpp-" + std::to_string(dist(mt)) + ".tmp";
}

} // anonymous namespace

///
/// Creates a temporary file with the given content.
///
TmpFile::TmpFile(const std::string& content):
		path{getUniqueTemporaryFilePath()} {
	std::ofstream file{path, std::ios::binary};
	file << content;
}

///
/// Removes the temporary file from the filesystem.
///
TmpFile::~TmpFile() {
	std::remove(path.c_str());
}

///
/// Returns a path to the file.
///
std::string TmpFile::getPath() const {
	return path;
}

///
/// Creates a temporary file with the given content.
///
std::unique_ptr<TmpFile> TmpFile::createWithContent(const std::string& content) {
	return std::make_unique<TmpFile>(content);
}

RemoveFileOnDestruction::RemoveFileOnDestruction(const std::string& path):
	path(path) {}

RemoveFileOnDestruction::~RemoveFileOnDestruction() {
	std::remove(path.c_str());
}

} // namespace tests
} // namespace ar
