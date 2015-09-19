///
/// @file      ar/internal/files/string_file.h
/// @copyright (c) 2015 by Petr Zemek (s3rvac@gmail.com) and contributors
/// @license   MIT, see the @c LICENSE file for more details
/// @brief     File storing its content in a string.
///

#ifndef AR_INTERNAL_FILES_STRING_FILE_H
#define AR_INTERNAL_FILES_STRING_FILE_H

#include <string>

#include "ar/file.h"

namespace ar {
namespace internal {

///
/// File storing its content in a string.
///
class StringFile: public File {
public:
	explicit StringFile(const std::string& content);
	StringFile(const std::string& content, const std::string& name);
	virtual ~StringFile() override;

	virtual std::string getName() const override;
	virtual std::string getContent() override;
	virtual void saveCopyTo(const std::string& directoryPath) override;
	virtual void saveCopyTo(const std::string& directoryPath,
		const std::string& name) override;

private:
	/// File content.
	std::string content;

	/// File name.
	std::string name;
};

} // namespace internal
} // namespace ar

#endif
