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

class File;
class Files;

namespace internal {

///
/// Extractor of files from an archive.
///
class Extractor {
public:
	Extractor();
	~Extractor();

	Files extract(std::unique_ptr<File> archive);

	/// @name Disabled
	/// @{
	Extractor(const Extractor&) = delete;
	Extractor(Extractor&&) = delete;
	Extractor& operator=(const Extractor&) = delete;
	Extractor& operator=(Extractor&&) = delete;
	/// @}

private:
	void readMagicString();
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

	void skipSpaces();
	std::size_t readNum();

private:
	/// Content of the archive.
	std::string content;

	/// Current index to @c content.
	std::size_t i;
};

} // namespace internal
} // namespace ar

#endif
