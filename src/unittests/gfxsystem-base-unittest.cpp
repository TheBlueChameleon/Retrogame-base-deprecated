#include <iostream>

#include "../globals.hpp"
#include "gfxsystem-base-unittest.hpp"

using namespace RetrogameBase;

bool unittest_loadFont() {
    std::cout << "TESTING FONT ENGINE" << std::endl;

    bool result = true;
    bool last;

    last = (fonts.size() == 3);
    if (last) {
        std::cout << "  three fonts prepared by default" << std::endl;
    } else {
        std::cout << "  unexpected count of default fonts, found " << fonts.size() << std::endl;
    }
    result &= last;

    last = true;
    for (auto & [name, fontPtr] : fonts) {
        std::cout << "  font with alias '" << name << "' ";
        if (static_cast<bool>(fontPtr)) {
            std::cout << "loaded correctly" << std::endl;
        } else {
            std::cout << "failed to load correctly" << std::endl;
            last = false;
        }
    }
    result &= last;


    return result;
}
