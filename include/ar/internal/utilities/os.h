///
/// @file      ar/internal/utilities/os.h
/// @copyright (c) 2015 by Petr Zemek (s3rvac@gmail.com) and contributors
/// @license   MIT, see the @c LICENSE file for more details
/// @brief     Operating-system-related utilities.
///

#ifndef AR_INTERNAL_UTILITIES_OS_H
#define AR_INTERNAL_UTILITIES_OS_H

#include <string>

// Are we on Windows?
#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) \
		|| defined(__WINDOWS__) || defined(__CYGWIN__)
#define AR_OS_WINDOWS
#endif

namespace ar {
namespace internal {

/// @name Operating System
/// @{

std::string fileNameFromPath(const std::string& path);
std::string readFile(const std::string& path);
void writeFile(const std::string& path, const std::string& content);
void copyFile(const std::string& srcPath, const std::string& dstPath);
std::string joinPaths(const std::string& path1, const std::string& path2);

/// @}

} // namespace internal
} // namespace ar

#endif
