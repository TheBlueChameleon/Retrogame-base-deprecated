// ========================================================================== //
// Depenencies

// STL
#include <iostream>

// own
#include "unittest-macros.hpp"
#include "../base/globals.hpp"
#include "globals-unittest.hpp"

using namespace RetrogameBase;

// ========================================================================== //
// tests

bool unittest_loadFont()
{
    std::cout << "TESTING FONT ENGINE" << std::endl;

    UNITTEST_VARS;

    // ...................................................................... //

    UNITTEST_ASSERT(
        fonts.size() == 3,
        "prepare three fonts by default"
    );

    for (auto & [name, fontPtr] : fonts)
    {
        UNITTEST_ASSERT(
            static_cast<bool>(fontPtr),
            "load font with alias '" << name << "' correctly"
        );
    }

    // ...................................................................... //

    UNITTEST_FINALIZE;
}
