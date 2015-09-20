///
/// @file      ar/internal/extractor.cpp
/// @copyright (c) 2015 by Petr Zemek (s3rvac@gmail.com) and contributors
/// @license   MIT, see the @c LICENSE file for more details
/// @brief     Implementation of the extractor of files from archives.
///

#include <cctype>

#include "ar/exceptions.h"
#include "ar/file.h"
#include "ar/internal/extractor.h"
#include "ar/internal/files/string_file.h"

using namespace std::literals::string_literals;

namespace ar {
namespace internal {

namespace {

const auto MagicString = "!<arch>\n"s;
const auto FileHeaderEnd = "`\n"s;

} // anonymous namespace

Extractor::Extractor():
	content(), i(0) {}

Extractor::~Extractor() = default;

///
/// Extracts files from the given archive content.
///
/// @throws InvalidArchiveError when the archive is invalid.
///
Files Extractor::extract(const std::string& archiveContent) {
	content = archiveContent;
	i = 0;

	readMagicString();
	auto files = readFiles();
	return files;
}

void Extractor::readMagicString() {
	if (content.substr(i, MagicString.size()) != MagicString) {
		throw InvalidArchiveError{"missing magic string"};
	}
	i += MagicString.size();
}

Files Extractor::readFiles() {
	Files files;
	while (i < content.size()) {
		files.push_back(readFile());
	}
	return files;
}

std::unique_ptr<File> Extractor::readFile() {
	auto fileName = readFileName();
	readFileTimestamp();
	readFileOwnerId();
	readFileGroupId();
	readFileMode();
	auto fileSize = readFileSize();
	readUntilEndOfFileHeader();
	auto fileContent = readFileContent(fileSize);

	return std::make_unique<StringFile>(fileContent, fileName);
}

std::string Extractor::readFileName() {
	auto pos = content.find('/', i);
	if (pos == std::string::npos) {
		throw InvalidArchiveError{"missing '/' after file name"};
	}

	auto fileName = content.substr(i, pos - i);
	i = pos + 1;
	return fileName;
}

void Extractor::readFileTimestamp() {
	skipSpaces();
	readNum();
}

void Extractor::readFileOwnerId() {
	skipSpaces();
	readNum();
}

void Extractor::readFileGroupId() {
	skipSpaces();
	readNum();
}

void Extractor::readFileMode() {
	skipSpaces();
	readNum();
}

std::size_t Extractor::readFileSize() {
	skipSpaces();
	return readNum();
}

void Extractor::readUntilEndOfFileHeader() {
	auto pos = content.find(FileHeaderEnd, i);
	if (pos == std::string::npos) {
		throw InvalidArchiveError{"missing end of file header"};
	}
	i = pos + FileHeaderEnd.size();
}

std::string Extractor::readFileContent(std::size_t fileSize) {
	auto fileContent = content.substr(i, fileSize);
	i += fileSize;
	return fileContent;
}

void Extractor::skipSpaces() {
	while (content[i] == ' ') {
		++i;
	}
}

std::size_t Extractor::readNum() {
	std::string numAsStr;
	while (std::isdigit(content[i])) {
		numAsStr += content[i];
		++i;
	}
	return std::stoull(numAsStr);
}

} // namespace internal
} // namespace ar
