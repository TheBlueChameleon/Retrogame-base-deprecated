// ========================================================================== //
// template function

/*
bool unittest_foobar()
{
    std::cout << "TESTING FOOBAR CLASS" << std::endl;

    UNITTEST_VARS;

    // ...................................................................... //

    UNITTEST_ASSERT(
        1 == 1,
        "assert basic logic"
        );

    // ...................................................................... //

    UNITTEST_FINALIZE;
}
*/

// ========================================================================== //
// true code

#ifndef UNITTESTMACROS_HPP
#define UNITTESTMACROS_HPP

// ========================================================================== //
// dependencies

#include <stdexcept>

#include <string>
#include <sstream>
#include <vector>

// ========================================================================== //
// macros

#define UNITTEST_VARS \
    bool              unittest_result      = true; \
    bool              unittest_last_result = true; \
    bool              unittest_captureCerr = false; \
    std::stringstream unittest_cerrBuffer; \
    std::streambuf*   unittest_originalCerr;

#define UNITTEST_FINALIZE \
    if (unittest_captureCerr) {UNITTEST_RELEASE_CERR;} \
    return unittest_result;

#define UNITTEST_CRITICAL_BARRIER \
    if (!unittest_last_result) { throw std::runtime_error("Inconsistent state of the unittest framework"); }

// .......................................................................... //

#define UNITTEST_ASSERT(expr, taskDescription) \
    unittest_last_result = expr; \
    if (unittest_last_result) {std::cout << "  succeeded to " << taskDescription << std::endl;} \
    else                      {std::cout << "  failed to " << taskDescription << std::endl;} \
    unittest_result &= unittest_last_result;

// .......................................................................... //

#define UNITTEST_THROWS(expr, exception, taskDescription) \
    try { \
        unittest_last_result = false; expr; \
    } catch (const exception& e) { \
        unittest_last_result = true; \
    } \
    if (unittest_last_result) {std::cout << "  succeeded to " << taskDescription << std::endl;} \
    else                      {std::cout << "  failed to "    << taskDescription << std::endl;} \
    unittest_result &= unittest_last_result;

#define UNITTEST_DOESNT_THROW(expr, exception, taskDescription) \
    try { \
        unittest_last_result = true; expr; \
    } catch (const exception& e) { \
        unittest_last_result = false; \
        std::cout << "  " << e.what() << std::endl; \
    } \
    if (unittest_last_result) {std::cout << "  succeeded to " << taskDescription << std::endl;} \
    else                      {std::cout << "  failed to " << taskDescription << std::endl;} \
    unittest_result &= unittest_last_result;

// .......................................................................... //

#define UNITTEST_CAPTURE_CERR \
    unittest_captureCerr  = true; \
    unittest_originalCerr = std::cerr.rdbuf(unittest_cerrBuffer.rdbuf());

#define UNITTEST_RELEASE_CERR \
    std::cerr.rdbuf( unittest_originalCerr );

#define UNITTEST_CLEAR_CERR \
    unittest_cerrBuffer.str("");

#define UNITTEST_ASSERT_STATE_CERR(cerrText, taskDescription) \
    UNITTEST_ASSERT(unittest_cerrBuffer.str() == cerrText, taskDescription); \
    if (!unittest_last_result) { \
        std::cout << "    found state:" << std::endl; \
        std::cout << "      '" << unittest_cerrBuffer.str() << "'"<< std::endl; \
        std::cout << "    expected state:" << std::endl; \
        std::cout << "      '" << cerrText << "'"<< std::endl; \
    }

#define UNITTEST_ASSERT_OUTPUTS_ON_CERR(expr, cerrText, taskDescription) \
    UNITTEST_CLEAR_CERR; \
    expr; \
    UNITTEST_ASSERT_STATE_CERR(cerrText, taskDescription)

#endif // UNITTESTMACROS_HPP

// ========================================================================== //
// type

using Unittest_RessorceList = std::vector<std::string>;

// ========================================================================== //
// procs

void unittest_check_files_present(const Unittest_RessorceList& files);
void unittest_check_directories  (const Unittest_RessorceList& directories);
