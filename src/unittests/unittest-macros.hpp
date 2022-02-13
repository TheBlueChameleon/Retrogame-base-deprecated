#ifndef UNITTESTMACROS_HPP
#define UNITTESTMACROS_HPP

#define UNITTEST_VARS bool result = true, last = true;
#define UNITTEST_FINALIZE return result;
#define UNITTEST_CRITICAL_BARRIER if (!last) {return result;}

#define UNITTEST_ASSERT(expr, taskDescription) {\
        last = expr; \
        if (last) {std::cout << "  succeeded to " << taskDescription << std::endl;} \
        else      {std::cout << "  failed to " << taskDescription << std::endl;} \
        result &= last; \
    }

#define UNITTEST_THROWS(expr, exception, taskDescription) {\
        try                        {last = false; expr;} \
        catch (const exception& e) {last = true;} \
        if (last) {std::cout << "  succeeded to " << taskDescription << std::endl;} \
        else      {std::cout << "  failed to " << taskDescription << std::endl;} \
        result &= last; \
    }

#define UNITTEST_DOESNT_THROW(expr, exception, taskDescription) {\
        try                        {last = true; expr;} \
        catch (const exception& e) {last = false;} \
        if (last) {std::cout << "  succeeded to " << taskDescription << std::endl;} \
        else      {std::cout << "  failed to " << taskDescription << std::endl;} \
        result &= last; \
    }

#endif // UNITTESTMACROS_HPP
