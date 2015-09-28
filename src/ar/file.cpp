///
/// @file      ar/file.cpp
/// @copyright (c) 2015 by Petr Zemek (s3rvac@gmail.com) and contributors
/// @license   MIT, see the @c LICENSE file for more details
/// @brief     Implementation of the representation and factory for files.
///

#include "ar/file.h"
#include "ar/internal/files/filesystem_file.h"
#include "ar/internal/files/string_file.h"

using namespace ar::internal;

namespace ar {

File::File() = default;

File::~File() = default;

/// @fn File::getName()
///
/// Returns the name of the file.
///
/// When the file has no name, the empty string is returned.
///

/// @fn File::getContent()
///
/// Returns the content of the file.
///

/// @fn File::saveCopyTo(const std::string& directoryPath)
///
/// Stores a copy of the file into the given directory.
///

/// @fn File::saveCopyTo(const std::string& directoryPath,
///     const std::string& name)
///
/// Stores a copy of the file into the given directory under the given name.
///

///
/// Returns a file containing the given content with the given name.
///
/// @param[in] content Content of the file.
/// @param[in] name Name of the file.
///
std::unique_ptr<File> File::fromContentWithName(const std::string& content,
		const std::string& name) {
	return std::make_unique<StringFile>(content, name);
}

///
/// Returns a file from the given path.
///
/// @param[in] path Path to the file.
///
/// The name of the file is obtained automatically.
///
std::unique_ptr<File> File::fromFilesystem(const std::string& path) {
	return std::make_unique<FilesystemFile>(path);
}

///
/// Returns a file from the given path, but with a custom name.
///
/// @param[in] path Path to the file.
/// @param[in] name Name to be used as the file's name.
///
/// Use this function only if you want to choose a different name for the file
/// than the one it already has.
///
std::unique_ptr<File> File::fromFilesystemWithOtherName(
		const std::string& path, const std::string& name) {
	return std::make_unique<FilesystemFile>(path, name);
}

///
/// Constructs an empty container (without files).
///
Files::Files() = default;

Files::Files(Files&& other): files{std::move(other.files)} {}

Files::~Files() = default;

///
/// Returns a reference to the first file in the container.
///
/// Calling this function on an empty container is undefined.
///
auto Files::front() -> reference {
	return files.front();
}

///
/// Returns a reference to the last file in the container.
///
/// Calling this function on an empty container is undefined.
///
auto Files::back() -> reference {
	return files.back();
}

auto Files::begin() noexcept -> iterator {
	return files.begin();
}

auto Files::end() noexcept -> iterator {
	return files.end();
}

///
/// Is the container empty?
///
bool Files::empty() const noexcept {
	return files.empty();
}

///
/// Returns the number of files in the container.
///
auto Files::size() const noexcept -> size_type {
	return files.size();
}

///
/// Appends the given file to the end of the container.
///
void Files::push_back(value_type file) {
	files.push_back(std::move(file));
}

} // namespace ar
