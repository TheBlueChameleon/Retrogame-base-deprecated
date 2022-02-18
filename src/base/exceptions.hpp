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
    ERROR(InvalidFile);
    ERROR(InvalidVersion);

    ERROR(MemoryManagement);

    ERROR(SdlInternal);
    ERROR(PugiInternal);
}

// ========================================================================== //
// end local macro

#undef ERROR

#endif // EXCEPTIONS_HPP
