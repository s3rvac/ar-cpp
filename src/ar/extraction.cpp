///
/// @file      ar/extraction.cpp
/// @copyright (c) 2015 by Petr Zemek (s3rvac@gmail.com) and contributors
/// @license   MIT, see the @c LICENSE file for more details
/// @brief     Implementation of archive extraction.
///

#include "ar/extraction.h"
#include "ar/file.h"
#include "ar/internal/extractor.h"

using namespace ar::internal;

namespace ar {

///
/// Extracts the given archive and returns the files it contains.
///
/// @throws InvalidArchiveError when the archive is invalid.
///
std::vector<std::unique_ptr<File>> extract(std::unique_ptr<File> archive) {
	Extractor extractor;
	return extractor.extract(std::move(archive));
}

} // namespace ar
