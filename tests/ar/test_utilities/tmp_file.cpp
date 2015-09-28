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
	std::random_device rd;
	std::mt19937 mt{rd()};
	std::uniform_int_distribution<std::size_t> dist;
	return "ar-cpp-" + std::to_string(dist(mt)) + ".tmp";
}

} // anonymous namespace

///
/// Private implementation of TmpFile.
///
struct TmpFile::Impl {
	/// Creates a temporary file with the given content.
	Impl():
		path{getUniqueTemporaryFilePath()},
		file{path, std::ios::binary} {}

	/// Path to the temporary file.
	const std::string path;

	/// Temporary file.
	std::ofstream file;
};

///
/// Creates a temporary file with the given content.
///
TmpFile::TmpFile(const std::string& content):
		impl{std::make_unique<Impl>()} {
	impl->file << content;
	impl->file.flush();
}

///
/// Destroys the temporary file by removing it.
///
TmpFile::~TmpFile() {
	std::remove(impl->path.c_str());
}

///
/// Returns a path to the file.
///
std::string TmpFile::getPath() const {
	return impl->path;
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
