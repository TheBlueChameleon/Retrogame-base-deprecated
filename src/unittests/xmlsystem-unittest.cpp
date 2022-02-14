// ========================================================================== //
// Depenencies

// STL
#include <iostream>
#include <stdexcept>

// Pugi
#include "../xmlSystem/pugixml.hpp"

// own
#include "unittest-macros.hpp"
#include "xmlsystem-unittest.hpp"

bool unittest_xmlSystem_load()
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

bool unittest_xmlSystem_extractAttributeList()
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
