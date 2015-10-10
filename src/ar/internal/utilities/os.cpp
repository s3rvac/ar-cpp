///
/// @file      ar/internal/utilities/os.cpp
/// @copyright (c) 2015 by Petr Zemek (s3rvac@gmail.com) and contributors
/// @license   MIT, see the @c LICENSE file for more details
/// @brief     Implementation of the operating-system-related utilities.
///

#include <algorithm>
#include <fstream>
#include <regex>

#include "ar/exceptions.h"
#include "ar/internal/utilities/os.h"

namespace ar {
namespace internal {

namespace {

bool isPathFromRoot(const std::string& path) {
#ifdef AR_OS_WINDOWS
	return std::regex_match(path, std::regex{R"([a-zA-Z]:(/|\\).*)"}) ||
		path.front() == '/';
#else
	return path.front() == '/';
#endif
}

bool endsWithSlash(const std::string& path) {
#ifdef AR_OS_WINDOWS
	return path.back() == '\\' || path.back() == '/';
#else
	return path.back() == '/';
#endif
}

#ifdef AR_OS_WINDOWS
bool containsForwardSlash(const std::string& path) {
	return path.find('/') != std::string::npos;
}
#endif

} // anonymous namespace

///
/// Returns the file name from the given path.
///
/// If there is no file name, it returns the empty string.
///
std::string fileNameFromPath(const std::string& path) {
	auto isPathSeparator = [](const auto c) {
#ifdef AR_OS_WINDOWS
		return c == '\\' || c == '/';
#else
		return c == '/';
#endif
	};

	return {
		std::find_if(path.rbegin(), path.rend(),
			isPathSeparator).base(),
		path.end()
	};
}

///
/// Joins the given paths.
///
std::string joinPaths(const std::string& path1, const std::string& path2) {
	if (path1.empty()) {
		return path2;
	}

	if (path2.empty()) {
		return path1;
	}

	if (isPathFromRoot(path2)) {
		return path2;
	}

	if (endsWithSlash(path1)) {
		return path1 + path2;
	}

#ifdef AR_OS_WINDOWS
	if (!containsForwardSlash(path1) && !containsForwardSlash(path2)) {
		return path1 + '\\' + path2;
	}
#endif

	return path1 + '/' + path2;
}

///
/// Returns the content of the given file.
///
/// @param[in] path Path to the file.
///
/// @throws IOError When the file cannot be opened or read.
///
/// The file is opened in the binary mode, so no conversions are performed
/// during the reading.
///
std::string readFile(const std::string& path) {
	std::ifstream file{path, std::ios::binary};
	if (!file) {
		throw IOError{"cannot open file \"" + path + "\""};
	}

	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	std::string content;
	try {
		// The following method gets the file size, resizes the string holding
		// the content, and reads the file afterwards. The obtained file size
		// is reliable because we have opened the file in the binary mode, so
		// there are no conversions performed.
		file.seekg(0, std::ios::end);
		content.resize(file.tellg());
		file.seekg(0, std::ios::beg);
		file.read(&content[0], content.size());
	} catch (const std::ifstream::failure& ex) {
		throw IOError{"cannot read file \"" + path + "\" (" + ex.what() + ")"};
	}
	return content;
}

///
/// Stores a file with the given @a content into the given @a path.
///
/// @throws IOError When the file cannot be opened or written.
///
/// The file is opened in the binary mode, so no conversions are performed
/// during writing.
///
void writeFile(const std::string& path, const std::string& content) {
	std::ofstream file{path, std::ios::binary};
	if (!file) {
		throw IOError{"cannot open file \"" + path + "\""};
	}

	file << content;
	if (!file) {
		throw IOError{"cannot write file \"" + path + "\""};
	}
}

///
/// Copies file in @a srcPath to a file in @a dstPath.
///
/// @throws IOError When a file cannot be opened, read, or written.
///
void copyFile(const std::string& srcPath, const std::string& dstPath) {
	auto content = readFile(srcPath);
	writeFile(dstPath, content);
}

} // namespace internal
} // namespace ar
