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
	initializeWith(archiveContent);
	readMagicString();
	readLookupTable();
	readFileNameTable();
	auto files = readFiles();
	return files;
}

void Extractor::initializeWith(const std::string& archiveContent) {
	content = archiveContent;
	i = 0;
	fileNameTable.clear();
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
	// However, we need to ensure that it is just a standalone '/' because "//"
	// denotes the start of a filename table.
	if (content[i] == '/' && content.substr(i, 2) != "//") {
		// The lookup table has the same format as a file. However, as we do
		// not need it, throw it away after reading (i.e. do not store its
		// content).
		++i;
		readFileTimestamp();
		readFileOwnerId();
		readFileGroupId();
		readFileMode();
		auto fileSize = readFileSize();
		readUntilEndOfFileHeader();
		readFileContent(fileSize);
	}
}

void Extractor::readFileNameTable() {
	// In the GNU format, the special file name "//" denotes a filename table.
	// It contains names of files, one by line, that are referenced by
	// subsequent file headers. It is used to store file names that are longer
	// than 16 chars.
	//
	// Example:
	//
	//   !<arch>\n
	//   //                                              42        `\n
	//   very_long_name_of_a_module_in_archive.o/\n
	//   \n
	//   /0              0           0     0     644     22        `\n
	//   contents of the module
	//
	// The references are of the form "/X", where X is the index into the
	// filename table.
	if (content.substr(i, 2) == "//") {
		i += 2;
		const auto tableSize = readNumber("filename table size");
		readUntilEndOfFileHeader();
		const auto tableStart = i;
		const auto tableEnd = i + tableSize;
		while (i < tableEnd) {
			readFileNameIntoFileNameTable(tableStart);
		}
	}
}

void Extractor::readFileNameIntoFileNameTable(std::size_t tableStart) {
	// A row in the filename table in the GNU variant is of the form
	//
	//   module.o/
	//
	const auto tableIndex = i - tableStart;
	auto fileName = readFileNameEndedWithSlash();
	fileNameTable.emplace(tableIndex, std::move(fileName));

	// Skip separators/padding.
	skipEndsOfLines();
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
	// In the GNU variant, the name of the file can be either an index into the
	// filename table:
	//
	//   /X
	//
	// or a slash-ended name:
	//
	//   module.o/
	//
	if (content[i] == '/') {
		++i;
		const auto index = readNumber("index into filename table");
		return nameFromFileNameTableOnIndex(index);
	} else {
		return readFileNameEndedWithSlash();
	}
}

std::string Extractor::readFileNameEndedWithSlash() {
	auto pos = content.find('/', i);
	ensureContainsSlashOnPosition(pos);
	auto fileName = content.substr(i, pos - i);
	i = pos + 1;
	return fileName;
}

std::string Extractor::nameFromFileNameTableOnIndex(std::size_t index) const {
	auto it = fileNameTable.find(index);
	ensureIsValidFileNameTableIndex(it, index);
	return it->second;
}

void Extractor::readFileTimestamp() {
	// Currently unused.
	readNumber("timestamp");
}

void Extractor::readFileOwnerId() {
	// Currently unused.
	readNumber("file owner ID");
}

void Extractor::readFileGroupId() {
	// Currently unused.
	readNumber("file group ID");
}

void Extractor::readFileMode() {
	// Currently unused.
	readNumber("file mode");
}

std::size_t Extractor::readFileSize() {
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
	skipSuccessiveChars(' ');
}

void Extractor::skipEndsOfLines() {
	skipSuccessiveChars('\n');
}

void Extractor::skipSuccessiveChars(char c) {
	while (content[i] == c) {
		++i;
	}
}

std::size_t Extractor::readNumber(const std::string& name) {
	skipSpaces();

	std::string numAsStr;
	while (std::isdigit(content[i])) {
		numAsStr += content[i];
		++i;
	}
	ensureNumberWasRead(numAsStr, name);
	return std::stoull(numAsStr);
}

void Extractor::ensureIsValidFileNameTableIndex(FileNameTable::const_iterator it,
		std::size_t index) const {
	if (it == fileNameTable.end()) {
		throw InvalidArchiveError{
			"invalid index into filename table: " + std::to_string(index)
		};
	}
}

void Extractor::ensureContainsSlashOnPosition(
		std::string::size_type pos) const {
	if (pos == std::string::npos) {
		throw InvalidArchiveError{"missing '/' after file name"};
	}
}

void Extractor::ensureContainsFileHeaderOnPosition(
		std::string::size_type pos) const {
	if (pos == std::string::npos) {
		throw InvalidArchiveError{"missing end of file header"};
	}
}

void Extractor::ensureContentOfGivenSizeWasRead(std::size_t readContentSize,
		std::size_t expectedContentSize) const {
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
		const std::string& name) const {
	if (numAsStr.empty()) {
		throw InvalidArchiveError{"missing number (" + name + ")"};
	}
}

} // namespace internal
} // namespace ar
