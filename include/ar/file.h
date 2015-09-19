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
#include <vector>

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

///
/// A vector-like container storing files.
///
class Files {
private:
	/// Underlying type of a container in which files are stored.
	using Container = std::vector<std::unique_ptr<File>>;

public:
	using size_type = Container::size_type;
	using value_type = Container::value_type;
	using reference = Container::reference;
	using iterator = Container::iterator;

public:
	Files();
	Files(Files&& other);
	~Files();

	/// @name File Access
	/// @{
	reference front();
	reference back();
	/// @}

	/// @name Iterators
	/// @{
	iterator begin();
	iterator end();
	/// @}

	/// @name Capacity
	/// @{
	bool empty() const noexcept;
	size_type size() const noexcept;
	/// @}

	/// @name Modifiers
	/// @{
	void push_back(value_type file);
	/// @}

private:
	Container files;
};

} // namespace ar

#endif
