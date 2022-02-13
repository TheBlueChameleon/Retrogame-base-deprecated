// ========================================================================== //
// Depenencies

// STL
#include <iostream>
#include <stdexcept>

// own
#include "unittest-macros.hpp"
#include "../globals.hpp"
#include "../gfxSystem/window.hpp"
#include "window-unittest.hpp"

using namespace RetrogameBase;

// ========================================================================== //
// tests

bool unittest_Window_CTor()
{
    std::cout << "TESTING WINDOW CTORS" << std::endl;

    UNITTEST_VARS;

    // ...................................................................... //

    UNITTEST_DOESNT_THROW(
        Window win("valid window"),
        std::exception,
        "set up default window"
    );

    UNITTEST_DOESNT_THROW(
        Window win(""),
        std::exception,
        "set up window with empty string as title"
    );

    UNITTEST_DOESNT_THROW(
        Window win(nullptr),
        std::exception,
        "set up window with nullptr as title"
    );

    // ...................................................................... //

    UNITTEST_DOESNT_THROW(
        Window win("null size", 0, 0),
        std::exception,
        "set up window with null size"
    );

    UNITTEST_DOESNT_THROW(
        Window win("negative size", -1, -1),
        std::exception,
        "set up window with negative size"
    );

    // ...................................................................... //

    UNITTEST_THROWS(
        Window win("null size", 80, 60, -1),
        std::runtime_error,
        "throw on set up window with all flags"
    );

    // ...................................................................... //

    UNITTEST_FINALIZE;
}

bool unittest_Window_Stores()
{
    std::cout << "TESTING WINDOW STORE INTERFACE" << std::endl;

    UNITTEST_VARS;

    // ...................................................................... //



    // ...................................................................... //

    UNITTEST_FINALIZE;
}
