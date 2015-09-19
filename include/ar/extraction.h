///
/// @file      ar/extraction.h
/// @copyright (c) 2015 by Petr Zemek (s3rvac@gmail.com) and contributors
/// @license   MIT, see the @c LICENSE file for more details
/// @brief     Archive extraction.
///

#ifndef AR_EXTRACTION_H
#define AR_EXTRACTION_H

#include <memory>

namespace ar {

class File;
class Files;

Files extract(std::unique_ptr<File> archive);

} // namespace ar

#endif
