///
/// @file      tools/ar-extract.cpp
/// @copyright (c) 2015 by Petr Zemek (s3rvac@gmail.com) and contributors
/// @license   MIT, see the @c LICENSE file for more details
/// @brief     A sample application that uses the library to extract archives.
///

#include <iostream>

#include "ar/ar.h"

using namespace ar;

int main(int argc, char** argv) {
	if (argc != 2) {
		std::cerr << "usage: " << argv[0] << " ARCHIVE\n";
		return 1;
	}

	try {
		auto files = extract(File::fromFilesystem(argv[1]));
		for (auto& file : files) {
			std::cout << file->getName() << "\n";
			file->saveCopyTo(".");
		}
		return 0;
	} catch (const Error& ex) {
		std::cerr << "error: " << ex.what() << "\n";
		return 1;
	}
}
