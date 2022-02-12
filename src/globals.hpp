#ifndef GLOBALS_HPP
#define GLOBALS_HPP

// ========================================================================== //
// dependencies

// STL
#include <string>
#include <unordered_map>

// SDL
#include <SDL2/SDL_ttf.h>

// ========================================================================== //
// namespace

namespace RetrogameBase {

// ========================================================================== //
// proc

    void initAll ();

    void initGlobals();
    void freeGlobals();

// ========================================================================== //
// colors

    constexpr SDL_Color
    color_white       = {255, 255, 255,   0},
    color_grey        = {128, 128, 128,   0},
    color_black       = {  0,   0,   0,   0},
    color_red         = {255,   0,   0,   0},
    color_green       = {  0, 255,   0,   0},
    color_blue        = {  0,   0, 255,   0},
    color_yellow      = {255, 255,   0,   0},
    color_purple      = {255,   0, 255,   0},
    color_cyan        = {  0, 255, 255,   0},
    color_dark_red    = {128,   0,   0,   0},
    color_dark_green  = {  0, 128,   0,   0},
    color_dark_blue   = {  0,   0, 128,   0},
    color_brown       = {128, 128,   0,   0},
    color_dark_purple = {128,   0, 128,   0},
    color_dark_cyan   = {  0, 128, 128,   0};

// ========================================================================== //
// fonts

    extern std::unordered_map<std::string, TTF_Font*> fonts;

// ========================================================================== //
// namespace

}

#endif // GLOBALS_HPP
