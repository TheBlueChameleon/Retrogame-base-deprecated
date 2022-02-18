#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

// ========================================================================== //
// dependencies

// STL
#include <stdexcept>
#include <string>

// ========================================================================== //
// local macro

#define ERROR(classname) class classname ## Error : public std::runtime_error { \
        public:\
            classname ## Error (const std::string& msg) : std::runtime_error(msg) {}\
    }

// ========================================================================== //
// Errors

namespace RetrogameBase
{
    ERROR(FileNotFound);
    //class FileNotFoundError   : std::runtime_error {};
    class InvalidFileError    : std::runtime_error {};
    class InvalidVersionError : std::runtime_error {};
}

// ========================================================================== //
// end local macro

#undef ERROR

#endif // EXCEPTIONS_HPP
