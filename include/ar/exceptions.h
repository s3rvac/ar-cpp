///
/// @file      ar/exceptions.h
/// @copyright (c) 2015 by Petr Zemek (s3rvac@gmail.com) and contributors
/// @license   MIT, see the @c LICENSE file for more details
/// @brief     Exceptions.
///

#ifndef AR_EXCEPTIONS_H
#define AR_EXCEPTIONS_H

#include <stdexcept>

namespace ar {

///
/// Base class for exceptions thrown by the library.
///
class Error: public std::runtime_error {
public:
	using std::runtime_error::runtime_error;
};

///
/// Exception thrown when the archive is invalid.
///
class InvalidArchiveError: public Error {
public:
	using Error::Error;
};

///
/// Exception thrown when there is an I/O error.
///
class IOError: public Error {
public:
	using Error::Error;
};

} // namespace ar

#endif
