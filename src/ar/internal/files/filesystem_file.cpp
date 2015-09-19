///
/// @file      ar/internal/files/filesystem_file.cpp
/// @copyright (c) 2015 by Petr Zemek (s3rvac@gmail.com) and contributors
/// @license   MIT, see the @c LICENSE file for more details
/// @brief     Implementation of the file stored in a filesystem.
///

#include "ar/internal/files/filesystem_file.h"
#include "ar/internal/utilities/os.h"

namespace ar {
namespace internal {

///
/// Constructs a file.
///
/// @param[in] path Path to the file in a filesystem.
///
FilesystemFile::FilesystemFile(const std::string& path):
	path{path}, name{fileNameFromPath(path)} {}

///
/// Constructs a file with a custom name.
///
/// @param[in] path Path to the file in a filesystem.
/// @param[in] name Name to be used as the file's name.
///
FilesystemFile::FilesystemFile(const std::string& path,
		const std::string& name):
	path{path}, name{name} {}

FilesystemFile::~FilesystemFile() = default;

std::string FilesystemFile::getName() const {
	return name;
}

std::string FilesystemFile::getContent() {
	return readFile(path);
}

void FilesystemFile::saveCopyTo(const std::string& directoryPath) {
	saveCopyTo(directoryPath, name);
}

void FilesystemFile::saveCopyTo(const std::string& directoryPath,
		const std::string& name) {
	copyFile(path, joinPaths(directoryPath, name));
}

} // namespace internal
} // namespace ar
