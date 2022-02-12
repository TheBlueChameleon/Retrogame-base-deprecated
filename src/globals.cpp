// ========================================================================== //
// dependencies

// STL
#include <cstdlib>
#include <iostream>

#include <string>
#include <unordered_map>

// SDL

// local
#include "globals.hpp"

// ========================================================================== //
// proc

void RetrogameBase::initAll() {
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

void RetrogameBase::initGlobals() {
    fonts.emplace(
        "fixed-small",
        TTF_OpenFont("../font/FreeMono.ttf", 8)
    );
    fonts.emplace(
        "fixed-medium",
        TTF_OpenFont("../font/FreeMono.ttf", 16)
    );
    fonts.emplace(
        "fixed-big",
        TTF_OpenFont("../font/FreeMono.ttf", 24)
    );

    std::atexit(freeGlobals);
}

void RetrogameBase::freeGlobals() {
    for (auto & [name, ptr] : fonts) {
        TTF_CloseFont(ptr);
    }

    //TTF_CloseFont(font_fixedSize);
}

// ========================================================================== //
// fonts

std::unordered_map<std::string, TTF_Font*> fonts;
