///
/// @file      ar/tests/test_utilities/tmp_file.h
/// @copyright (c) 2015 by Petr Zemek (s3rvac@gmail.com) and contributors
/// @license   MIT, see the @c LICENSE file for more details
/// @brief     Temporary-file utilities.
///

#ifndef AR_TESTS_TEST_UTILITIES_TMP_FILE_H
#define AR_TESTS_TEST_UTILITIES_TMP_FILE_H

#include <memory>
#include <string>

namespace ar {
namespace tests {

///
/// A temporary file that is automatically deleted when destructed.
///
class TmpFile {
public:
	TmpFile(const std::string& content);
	~TmpFile();

	std::string getPath() const;

	static std::unique_ptr<TmpFile> createWithContent(
		const std::string& content);

private:
	struct Impl;
	/// Private implementation.
	std::unique_ptr<Impl> impl;
};

///
/// RAII helper that removes the given file in its destructor.
///
class RemoveFileOnDestruction {
public:
	RemoveFileOnDestruction(const std::string& path);
	~RemoveFileOnDestruction();

private:
	/// Path to the file to be removed.
	const std::string path;
};

} // namespace tests
} // namespace ar

#endif
