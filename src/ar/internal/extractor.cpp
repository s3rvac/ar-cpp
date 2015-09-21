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
	readLookupTable();
	auto files = readFiles();
	return files;
}

void Extractor::readMagicString() {
	// The magic string should appear at the beginning of every archive.
	if (content.substr(i, MagicString.size()) != MagicString) {
		throw InvalidArchiveError{"missing magic string"};
	}
	i += MagicString.size();
}

void Extractor::readLookupTable() {
	// In the GNU format, the special file name '/' denotes a lookup table.
	if (content[i] == '/') {
		// The lookup table has the same format as a file. However, as we do
		// not need it, throw it away after reading (i.e. do not store the
		// result of readFile()).
		readFile();
	}
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
	// In the GNU variant, a file's name ends with a slash.
	auto pos = content.find('/', i);
	ensureContainsSlashOnPosition(pos);
	auto fileName = content.substr(i, pos - i);
	i = pos + 1;
	return fileName;
}

void Extractor::readFileTimestamp() {
	// Currently unused.
	skipSpaces();
	readNumber("timestamp");
}

void Extractor::readFileOwnerId() {
	// Currently unused.
	skipSpaces();
	readNumber("file owner ID");
}

void Extractor::readFileGroupId() {
	// Currently unused.
	skipSpaces();
	readNumber("file group ID");
}

void Extractor::readFileMode() {
	// Currently unused.
	skipSpaces();
	readNumber("file mode");
}

std::size_t Extractor::readFileSize() {
	skipSpaces();
	return readNumber("file size");
}

void Extractor::readUntilEndOfFileHeader() {
	auto pos = content.find(FileHeaderEnd, i);
	ensureContainsFileHeaderOnPosition(pos);
	i = pos + FileHeaderEnd.size();
}

std::string Extractor::readFileContent(std::size_t fileSize) {
	auto fileContent = content.substr(i, fileSize);
	ensureContentOfGivenSizeWasRead(fileContent.size(), fileSize);
	i += fileSize;
	return fileContent;
}

void Extractor::skipSpaces() {
	while (content[i] == ' ') {
		++i;
	}
}

std::size_t Extractor::readNumber(const std::string& name) {
	std::string numAsStr;
	while (std::isdigit(content[i])) {
		numAsStr += content[i];
		++i;
	}
	ensureNumberWasRead(numAsStr, name);
	return std::stoull(numAsStr);
}

void Extractor::ensureContainsSlashOnPosition(std::string::size_type pos) {
	if (pos == std::string::npos) {
		throw InvalidArchiveError{"missing '/' after file name"};
	}
}

void Extractor::ensureContainsFileHeaderOnPosition(std::string::size_type pos) {
	if (pos == std::string::npos) {
		throw InvalidArchiveError{"missing end of file header"};
	}
}

void Extractor::ensureContentOfGivenSizeWasRead(std::size_t readContentSize,
		std::size_t expectedContentSize) {
	if (readContentSize != expectedContentSize) {
		throw InvalidArchiveError{
			"premature end of file (expected " +
			std::to_string(expectedContentSize) +
			" bytes, read " +
			std::to_string(readContentSize) +
			" bytes)"
		};
	}
}

void Extractor::ensureNumberWasRead(const std::string& numAsStr,
		const std::string& name) {
	if (numAsStr.empty()) {
		throw InvalidArchiveError{"missing number (" + name + ")"};
	}
}

} // namespace internal
} // namespace ar
