///
/// @file      ar/file.h
/// @copyright (c) 2015 by Petr Zemek (s3rvac@gmail.com) and contributors
/// @license   MIT, see the @c LICENSE file for more details
/// @brief     Representation and factory for files.
///

#ifndef AR_FILE_H
#define AR_FILE_H

#include <memory>
#include <string>

namespace ar {

///
/// Base class and factory for files.
///
class File {
public:
	virtual ~File() = 0;

	virtual std::string getName() const = 0;
	virtual std::string getContent() = 0;
	virtual void saveCopyTo(const std::string& directoryPath) = 0;
	virtual void saveCopyTo(const std::string& directoryPath,
		const std::string& name) = 0;

	static std::unique_ptr<File> fromContentWithName(
		const std::string& content, const std::string& name);
	static std::unique_ptr<File> fromFilesystem(const std::string& path);
	static std::unique_ptr<File> fromFilesystemWithOtherName(
		const std::string& path, const std::string& name);

	/// @name Disabled
	/// @{
	File(const File&) = delete;
	File(File&&) = delete;
	File& operator=(const File&) = delete;
	File& operator=(File&&) = delete;
	/// @}

protected:
	File();
};

} // namespace ar

#endif
