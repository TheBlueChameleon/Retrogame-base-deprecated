#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

// ========================================================================== //
// dependencies

// STL
#include <stdexcept>
#include <string>

// ========================================================================== //
// local macro

#define ERROR(classname) class classname : public std::runtime_error { \
        public:\
            classname (const std::string& msg) : std::runtime_error(msg) {}\
    }

// ========================================================================== //
// Errors

namespace RetrogameBase
{
    ERROR(SdlInternalError);
    ERROR(PugiInternalError);

    ERROR(MemoryManagementError);

    ERROR(FileNotFoundError);
    ERROR(InvalidFileError);
    ERROR(InvalidVersionError);

    ERROR(InvalidFrameError);
}

// ========================================================================== //
// end local macro

#undef ERROR

#endif // EXCEPTIONS_HPP
