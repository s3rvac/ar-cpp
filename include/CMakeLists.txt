##
## Project:   ar-cpp
## Copyright: (c) 2015 by Petr Zemek <s3rvac@gmail.com> and contributors
## License:   MIT, see the LICENSE file for more details
##
## CMake configuration file for the include files.
##

set(PUBLIC_INCLUDES
	ar/ar.h
	ar/exceptions.h
	ar/extraction.h
	ar/file.h
)

install(FILES ${PUBLIC_INCLUDES} DESTINATION "${INSTALL_INCLUDE_DIR}/ar")
