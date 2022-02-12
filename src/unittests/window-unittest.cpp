// ========================================================================== //
// Depenencies

// STL
#include <iostream>
#include <stdexcept>

// own
#include "../globals.hpp"
#include "../gfxSystem/window.hpp"
#include "window-unittest.hpp"

using namespace RetrogameBase;

// ========================================================================== //
// tests


bool unittest_window_CTorDTor() {
    std::cout << "TESTING WINDOW CTors" << std::endl;
    bool result = true;
    bool last = true;
    // ...................................................................... //

    try {
        Window win("valid window");
    }  catch (const std::runtime_error& e) {
        last = false;
    }
    if (last) {
        std::cout << "  succeeded to set up default window" << std::endl;
    } else {
        std::cout << "  failed to set up default window" << std::endl;
    }
    result &= last;
    // ...................................................................... //

    try {
        Window win("");
    }  catch (const std::runtime_error& e) {
        last = false;
    }
    if (last) {
        std::cout << "  succeeded to set up window with empty string as title" << std::endl;
    } else {
        std::cout << "  failed to set up window with empty string as title" << std::endl;
    }
    result &= last;
    // ...................................................................... //

    try {
        Window win(nullptr);
    }  catch (const std::runtime_error& e) {
        last = false;
    }
    if (last) {
        std::cout << "  succeeded to set up window with nullptr as title" << std::endl;
    } else {
        std::cout << "  failed to set up window with nullptr as title" << std::endl;
    }
    result &= last;
    // ...................................................................... //

    try {
        Window win("null size", 0, 0);
    }  catch (const std::runtime_error& e) {
        last = false;
    }
    if (last) {
        std::cout << "  succeeded to set up window with null size" << std::endl;
    } else {
        std::cout << "  failed to set up window with null size" << std::endl;
    }
    result &= last;
    // ...................................................................... //

    try {
        Window win("null size", -1, -1);
    }  catch (const std::runtime_error& e) {
        last = false;
    }
    if (last) {
        std::cout << "  succeeded to set up window with negative size" << std::endl;
    } else {
        std::cout << "  failed to set up window with negative size" << std::endl;
    }
    result &= last;
    // ...................................................................... //

    try {
        Window win("null size", 80, 60, -1);
        last = false;
    }  catch (const std::runtime_error& e) {
        last = true;
    }
    if (last) {
        std::cout << "  succeeded to throw on set up window with all flags" << std::endl;
    } else {
        std::cout << "  failed to throw on set up window with all flags" << std::endl;
    }
    result &= last;

    return result;
}
