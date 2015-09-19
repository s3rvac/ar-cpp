///
/// @file      ar/internal/files/string_file.cpp
/// @copyright (c) 2015 by Petr Zemek (s3rvac@gmail.com) and contributors
/// @license   MIT, see the @c LICENSE file for more details
/// @brief     Implementation of the file storing its content in a string.
///

#include <utility>

#include "ar/internal/files/string_file.h"
#include "ar/internal/utilities/os.h"

namespace ar {
namespace internal {

///
/// Constructs a file with the given content.
///
StringFile::StringFile(const std::string& content):
	content{content} {}

///
/// Constructs a file with the given content and name.
///
StringFile::StringFile(const std::string& content, const std::string& name):
	content{content}, name{name} {}

StringFile::~StringFile() = default;

std::string StringFile::getName() const {
	return name;
}

std::string StringFile::getContent() {
	return content;
}

void StringFile::saveCopyTo(const std::string& directoryPath) {
	saveCopyTo(directoryPath, name);
}

void StringFile::saveCopyTo(const std::string& directoryPath,
		const std::string& name) {
	writeFile(joinPaths(directoryPath, name), getContent());
}

} // namespace internal
} // namespace ar
