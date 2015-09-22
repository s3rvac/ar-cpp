///
/// @file      ar/internal/extractor.h
/// @copyright (c) 2015 by Petr Zemek (s3rvac@gmail.com) and contributors
/// @license   MIT, see the @c LICENSE file for more details
/// @brief     Extractor of files from an archive.
///

#ifndef AR_INTERNAL_EXTRACTOR_H
#define AR_INTERNAL_EXTRACTOR_H

#include <cstdint>
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
	void initializeWith(const std::string& archiveContent);

	/// @name Reading
	/// @{
	void readMagicString();
	void readLookupTable();
	Files readFiles();
	std::unique_ptr<File> readFile();
	std::string readFileName();
	void readFileTimestamp();
	void readFileOwnerId();
	void readFileGroupId();
	void readFileMode();
	std::size_t readFileSize();
	void readUntilEndOfFileHeader();
	std::string readFileContent(std::size_t fileSize);

	/// @name Utilities
	/// @{
	void skipSpaces();
	std::size_t readNumber(const std::string& name);
	/// @}

	/// @name Validation
	/// @{
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
};

} // namespace internal
} // namespace ar

#endif
