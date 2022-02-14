/* template function
 *
 *

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

#ifndef UNITTESTMACROS_HPP
#define UNITTESTMACROS_HPP

#include <stdexcept>

#define UNITTEST_VARS bool unittest_result = true, unittest_last_result = true;
#define UNITTEST_FINALIZE return unittest_result;
#define UNITTEST_CRITICAL_BARRIER if (!unittest_last_result) { throw std::runtime_error("Inconsistent state of the unittest framework"); }

#define UNITTEST_ASSERT(expr, taskDescription) {\
        unittest_last_result = expr; \
        if (unittest_last_result) {std::cout << "  succeeded to " << taskDescription << std::endl;} \
        else                      {std::cout << "  failed to " << taskDescription << std::endl;} \
        unittest_result &= unittest_last_result; \
    }

#define UNITTEST_THROWS(expr, exception, taskDescription) {\
        try                        {unittest_last_result = false; expr;} \
        catch (const exception& e) {unittest_last_result = true;} \
        if (unittest_last_result) {std::cout << "  succeeded to " << taskDescription << std::endl;} \
        else                      {std::cout << "  failed to " << taskDescription << std::endl;} \
        unittest_result &= unittest_last_result; \
    }

#define UNITTEST_DOESNT_THROW(expr, exception, taskDescription) {\
        try                        {unittest_last_result = true; expr;} \
        catch (const exception& e) {unittest_last_result = false;} \
        if (unittest_last_result) {std::cout << "  succeeded to " << taskDescription << std::endl;} \
        else                      {std::cout << "  failed to " << taskDescription << std::endl;} \
        unittest_result &= unittest_last_result; \
    }

#endif // UNITTESTMACROS_HPP




