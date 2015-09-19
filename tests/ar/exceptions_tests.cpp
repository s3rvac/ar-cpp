///
/// @file      ar/exceptions_tests.cpp
/// @copyright (c) 2015 by Petr Zemek (s3rvac@gmail.com) and contributors
/// @license   MIT, see the @c LICENSE file for more details
/// @brief     Tests for the @c exceptions module.
///

#include <gtest/gtest.h>

#include "ar/exceptions.h"

using namespace std::literals::string_literals;

namespace ar {
namespace tests {

///
/// Tests for Error.
///
class ErrorTests: public testing::Test {};

TEST_F(ErrorTests,
WhatReturnsCorrectMessage) {
	const auto RefErrorMessage = "error message"s;
	Error ex{RefErrorMessage};

	ASSERT_EQ(RefErrorMessage, ex.what());
}

} // namespace tests
} // namespace ar
