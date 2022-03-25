#ifndef GLOBALS_HPP
#define GLOBALS_HPP

// ========================================================================== //
// dependencies

// STL
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>

#include <numbers>

// SDL
#include <SDL2/SDL_ttf.h>

// own
#include "exceptions.hpp"

// ========================================================================== //
// namespace

namespace RetrogameBase
{

// ========================================================================== //
// SDL ressources

    void initAll();

    void initGlobals();
    void freeGlobals();

// ========================================================================== //
// convenience

    bool isInteger (const std::string& str);

    std::vector<std::string_view> split_nonowning(
        const std::string_view& text,
        const std::string_view& separator=","
    );
    std::string_view trim_nonowning(
        const std::string_view& text,
        const std::string_view& charsToTrim = " \t\n"
    );

    inline double deg2rad(double deg)
    {
        return (deg / 180.) * std::numbers::pi;
    }

    inline bool isBetween(double x, double a, double b)
    {
        return (a <= x) && (x <= b);
    }

// ========================================================================== //
// colors

    constexpr SDL_Color
    color_white       = {255, 255, 255, 255},
    color_grey        = {128, 128, 128, 255},
    color_black       = {  0,   0,   0, 255},
    color_red         = {255,   0,   0, 255},
    color_green       = {  0, 255,   0, 255},
    color_blue        = {  0,   0, 255, 255},
    color_yellow      = {255, 255,   0, 255},
    color_purple      = {255,   0, 255, 255},
    color_cyan        = {  0, 255, 255, 255},
    color_dark_red    = {128,   0,   0, 255},
    color_dark_green  = {  0, 128,   0, 255},
    color_dark_blue   = {  0,   0, 128, 255},
    color_brown       = {128, 128,   0, 255},
    color_dark_purple = {128,   0, 128, 255},
    color_dark_cyan   = {  0, 128, 128, 255},
    color_transparent = {  0,   0,   0,   0};

    SDL_Color getColorFromHSL(double hue, double saturation, double lightness);

// ========================================================================== //
// trigonometry

    extern double valuesSin[360], valuesCos[360];

// ========================================================================== //
// fonts

    void loadFont(const std::string& alias, const std::string& filename, int size);

    extern std::unordered_map<std::string, TTF_Font*> fonts;

// ========================================================================== //
// user defined events

    extern Uint32 SDL_UserEventID;

    void pushUserEvent(Sint32 eventCode, void* eventData);

// ========================================================================== //
// SDL helper funcs

    SDL_Color getPixelFromSurface(SDL_Surface* surface, int x, int y);

// ========================================================================== //
// namespace

}

#endif // GLOBALS_HPP
