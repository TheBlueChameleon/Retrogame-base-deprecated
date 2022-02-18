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

    UNITTEST_THROWS(
        loadFont("alias", "nonexistent file", 12),
        FileNotFoundError,
        "throw on loading nonexistent file"
    );

    UNITTEST_THROWS(
        loadFont("fixed-small", "../font/FreeMono.ttf", 8),
        MemoryManagementError,
        "throw on loading font again"
    );

    // ...................................................................... //

    UNITTEST_FINALIZE;
}
