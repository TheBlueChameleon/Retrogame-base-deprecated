// ========================================================================== //
// dependencies

// STL
#include <exception>

#include <cstdlib>
#include <iostream>

#include <string>
#include <unordered_map>

using namespace std::string_literals;


// SDL

// local
#include "globals.hpp"

// ========================================================================== //
// namespace

namespace RetrogameBase {

// ========================================================================== //
// fonts

    std::unordered_map<std::string, TTF_Font*> fonts;

// ========================================================================== //
// proc

    void initAll() {
        if ( SDL_Init(SDL_INIT_EVERYTHING) ) {
            std::cerr << "error initializing SDL: " << SDL_GetError() << std::endl;
            std::exit(EXIT_FAILURE);
        } else {
            std::atexit(SDL_Quit);
        }

        if (TTF_Init() ) {
            std::cerr << "error initializing SDL: " << SDL_GetError() << std::endl;
            std::exit(EXIT_FAILURE);
        } else {
            std::atexit(TTF_Quit);
        }

        initGlobals();
    }

    void initGlobals() {
        loadFont("fixed-small","../font/FreeMono.ttf", 8);
        loadFont("fixed-medium","../font/FreeMono.ttf", 16);
        loadFont("fixed-big","../font/FreeMono.ttf", 24);

        std::atexit(freeGlobals);
    }

    void freeGlobals() {
        for (auto & [name, fontPtr] : fonts) {
            TTF_CloseFont(fontPtr);
        }
    }

    void loadFont(const std::string& alias, const std::string& filename, int size) {
        auto fontPtr = TTF_OpenFont(filename.c_str(), size);

        if (!fontPtr) {
            throw std::runtime_error("Could not load font '"s + filename + "'");
        }

        auto [it, success] = fonts.emplace(alias, fontPtr);

        if (!success) {
            TTF_CloseFont(fontPtr);
            throw std::runtime_error("Font '"s + filename + "' already in font store");
        }
    }


// ========================================================================== //
// namespace
}
