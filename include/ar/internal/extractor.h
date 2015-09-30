///
/// @file      ar/internal/extractor.h
/// @copyright (c) 2015 by Petr Zemek (s3rvac@gmail.com) and contributors
/// @license   MIT, see the @c LICENSE file for more details
/// @brief     Extractor of files from an archive.
///

#ifndef AR_INTERNAL_EXTRACTOR_H
#define AR_INTERNAL_EXTRACTOR_H

#include <cstddef>
#include <map>
#include <memory>
#include <string>

namespace ar {

class Files;

namespace internal {

///
/// Extractor of files from an archive.
///
class Extractor {
public:
	Extractor();
	~Extractor();

	Files extract(const std::string& archiveContent);

	/// @name Disabled
	/// @{
	Extractor(const Extractor&) = delete;
	Extractor(Extractor&&) = delete;
	Extractor& operator=(const Extractor&) = delete;
	Extractor& operator=(Extractor&&) = delete;
	/// @}

private:
	/// Mapping of an index into a file name.
	using FileNameTable = std::map<std::size_t, std::string>;

private:
	void initializeWith(const std::string& archiveContent);

	/// @name Reading
	/// @{
	void readMagicString();
	void readLookupTable();
	bool hasLookupTableAt(std::size_t i) const;
	void readFileNameTable();
	void readFileNameIntoFileNameTable(std::size_t startOfTable);
	Files readFiles();
	std::unique_ptr<File> readFile();
	std::string readFileName();
	bool hasNameSpecifiedViaIndexIntoFileNameTableAt(std::size_t j) const;
	std::string readFileNameEndedWithSlash();
	std::string nameFromFileNameTableOnIndex(std::size_t index) const;
	void readFileTimestamp();
	void readFileOwnerId();
	void readFileGroupId();
	void readFileMode();
	std::size_t readFileSize();
	void readUntilEndOfFileHeader();
	std::string readFileContent(std::size_t fileSize);

	/// @name Utilities
	/// @{
	bool isValid(std::size_t j) const noexcept;
	std::string::value_type charAt(std::size_t j) const;
	void skipSpaces();
	void skipEndsOfLines();
	void skipSuccessiveChars(char c);
	std::size_t readNumber(const std::string& name);
	/// @}

	/// @name Validation
	/// @{
	void ensureFileNameIsNonEmpty(const std::string& fileName) const;
	void ensureIsValidFileNameTableIndex(FileNameTable::const_iterator it,
		std::size_t index) const;
	void ensureContainsSlashOnPosition(std::string::size_type pos) const;
	void ensureContainsFileHeaderOnPosition(std::string::size_type pos) const;
	void ensureContentOfGivenSizeWasRead(std::size_t readContentSize,
		std::size_t expectedContentSize) const;
	void ensureNumberWasRead(const std::string& numAsStr,
		const std::string& name) const;
	/// @}

private:
	/// Content of the archive.
	std::string content;

	/// Current index to @c content.
	std::size_t i;

	/// Table containing names of files.
	FileNameTable fileNameTable;
};

} // namespace internal
} // namespace ar

#endif
