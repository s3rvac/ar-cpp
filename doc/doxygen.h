///
/// @file      doxygen.h
/// @copyright (c) 2015 by Petr Zemek (s3rvac@gmail.com) and contributors
/// @license   MIT, see the @c LICENSE file for more details
/// @brief     Documentation of the main page and the used namespaces.
///

namespace ar {

/**
@mainpage

@tableofcontents

This is an automatically generated API documentation for the <a href="https://github.com/s3rvac/ar-cpp">ar-cpp library</a>.

@section SectionGeneralInformation General Information

We start by giving you some general information concerning the library. Then, we describe how you can use library.

@subsection SectionNamingConventions Naming Conventions

The library uses the following naming conventions:
<ul>
    <li>Include files are named by using <code>snake_case</code>, e.g. <code>%extract.h</code>.</li>
    <li>Functions and variables are named by using <code>camelCase</code>, e.g. <code>%extract()</code>.</li>
    <li>Classes and constants are named by using <code>CamelCase</code>, e.g. <code>%File</code>.</li>
</ul>

@subsection SectionIncludes Includes

The easiest way to use the library is to include the general header file <code>ar.h</code> that includes all the necessary header files:
@code
#include <ar/ar.h>
@endcode
However, if you use only specific parts of the library, you can include just the used header files:
@code
#include <ar/extraction.h>
#include <ar/file.h>
// ...
@endcode

@subsection SectionNamespaces Namespaces

All the classes, functions, and constants the library provides are in the <code>\ref ar</code> namespace.

For simplicity, in this API documentation, this namespace is omitted, e.g. it uses just <code>File</code> instead of <code>ar::File</code>.

@subsection SectionErrorHandling Error Handling

The library uses exceptions to signal errors. The base class of all custom exceptions thrown by the library is Error. Currently, the following Error subclasses may be thrown:
- InvalidArchiveError -- When the input archive is invalid (e.g. corrupted headers).
- IOError -- When there is an I/O error (e.g. an archive cannot be read).

@section SectionLibraryUsage Library Usage

The present section describes how to use the library to work with archives.

@subsection SectionExtractionOfArchives Extraction of Archives

To extract an archive, call extract() and pass it your archive:
@code
auto files = extract(File::fromFilesystem("/path/to/archive.a"))
@endcode
Also, if you need, you can create an archive from a given content and file name, both given as a @c std::string:
@code
auto files = extract(File::fromContentWithName(content, name))
@endcode
The above call creates a virtual file in memory that does not correspond to any real file on your filesystem.

@subsection SectionWorkingWithFiles Working With Files

The extract() function returns a vector-like container of the extracted files. Each file is a subclass of File. You can iterate over the files, obtain info about them, and store them to the filesystem. For example, the following snippet prints the name of each file to the standard output and saves the extracted files to the current working directory:
@code
for (auto& file : files) {
    std::cout << file->getName() << "\n";
    file->saveCopyTo(".");
}
@endcode
See the File class for a complete list of member functions it provides.

@subsection SectionCompleteExample A Complete Example

The following example extracts the given archive to the current working directory and prints the name of each extracted file.
@code
#include <iostream>
#include <ar/ar.h>

using namespace ar;

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " ARCHIVE\n";
        return 1;
    }

    try {
        auto files = extract(File::fromFilesystem(argv[0]));
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
@endcode

@section SectionContact Contact

If you have any remarks or questions concerning the library, feel free to <a href="https://petrzemek.net" title="Contact me">contact me</a>.
*/

// Document the used namespaces (there is no better place).
/// @namespace ar The namespace of the library.
/// @namespace ar::internal Internal parts of the library.

} // namespace ar
