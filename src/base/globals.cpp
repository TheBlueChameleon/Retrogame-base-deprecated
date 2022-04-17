// ========================================================================== //
// dependencies

// STL
#include <exception>
#include <iostream>

#include <unordered_map>
#include <string>
#include <string_view>
using namespace std::string_literals;

#include <cstdlib>
#include <cctype>
#include <cmath>

#include <algorithm>
#include <filesystem>
namespace fs = std::filesystem;

// own
#include "globals.hpp"

// ========================================================================== //
// local macro

#define THROWTEXT(msg) ("RUNTIME EXCEPTION IN "s + (__PRETTY_FUNCTION__) + "\n"s + msg)

#define CHECK_FILE_EXISTS(filename) { \
        if (!fs::exists(filename)) { \
            throw FileNotFoundError(THROWTEXT("  file not found: '"s + filename + "'")); \
        } \
    }

// ========================================================================== //
// namespace

namespace RetrogameBase
{

// ========================================================================== //
// local proc definition

    void initValuesTrigonometry();

// ========================================================================== //
// SDL ressources

    void initAll()
    {
        if ( SDL_Init(SDL_INIT_EVERYTHING) )
        {
            std::cerr << "error initializing SDL: " << SDL_GetError() << std::endl;
            std::exit(EXIT_FAILURE);
        }
        else
        {
            std::atexit(SDL_Quit);
        }

        if (TTF_Init() )
        {
            std::cerr << "error initializing SDL: " << SDL_GetError() << std::endl;
            std::exit(EXIT_FAILURE);
        }
        else
        {
            std::atexit(TTF_Quit);
        }

        initGlobals();
        initValuesTrigonometry();
    }

    void initGlobals()
    {
        std::srand(std::time(nullptr));

        loadFont("fixed-small","../font/FreeMono.ttf", 8);
        loadFont("fixed-medium","../font/FreeMono.ttf", 16);
        loadFont("fixed-big","../font/FreeMono.ttf", 24);

        SDL_UserEventID = SDL_RegisterEvents(1);
        if ( SDL_UserEventID == static_cast<Uint32>(-1) )
        {
            std::cerr << "error registering SDL Userdefined Event" << std::endl;
            std::exit(EXIT_FAILURE);
        }

        std::atexit(freeGlobals);
    }

    void freeGlobals()
    {
        for (auto & [name, fontPtr] : fonts)
        {
            TTF_CloseFont(fontPtr);
        }
    }

// ========================================================================== //
// convenience

    bool isInteger (const std::string& stringToTest)
    {
        // *INDENT-OFF*
        if (stringToTest.empty())                                                               {return false;}
        if (!std::isdigit(stringToTest[0]) && stringToTest[0] != '+' && stringToTest[0] != '-') {return false;}
        // *INDENT-ON*

        auto remainingString = std::string_view(stringToTest.begin() + 1, stringToTest.end());
        return std::all_of(remainingString.begin(), remainingString.end(), ::isdigit);
    }

    std::vector<std::string_view> split_nonowning(
        const std::string_view& text,
        const std::string_view& separator
    )
    {
        std::vector<std::string_view> reVal;

        if (text.empty())
        {
            return reVal;
        }

        if (separator.empty())
        {
            reVal.push_back(text);
            return reVal;
        }

        const auto startIterator = text.begin();
        const auto skipWidth     = separator.length();
        auto last = 0;
        auto next = -skipWidth;

        while (true)
        {
            last = next + skipWidth;
            next = text.find(separator, last);

            if (next != std::string::npos)
            {
                reVal.emplace_back( startIterator + last, startIterator + next );
            }
            else
            {
                reVal.emplace_back( startIterator + last, text.end() );
                break;
            }
        }

        return reVal;
    }

    std::string_view trim_nonowning(
        const std::string_view& text,
        const std::string_view& charsToTrim)
    {
        if (text.length())
        {
            const auto startIterator = text.begin();
            return std::string_view(
                       startIterator + text.find_first_not_of(charsToTrim),
                       startIterator + text.find_last_not_of(charsToTrim) + 1
                   );
        }
        else
        {
            return std::string_view();
        }
    }

// ========================================================================== //
// colors

    SDL_Color getColorFromHSL(double H, double S, double L)
    {
        // formulae from https://www.rapidtables.com/convert/color/hsl-to-rgb.html

        double C = (1 - std::abs(2 * L - 1)) * S;
        double X = C * (1 - std::abs(std::fmod(H / 60.0, 2.0) - 1));
        double m = L - C/2.0;

        double r = 0;
        double g = 0;
        double b = 0;

        if      (isBetween(H,   0,  60))
        {
            r = C;
            g = X;
        }
        else if (isBetween(H,  60, 120))
        {
            r = X;
            g = C;
        }
        else if (isBetween(H, 120, 180))
        {
            g = C;
            b = X;
        }
        else if (isBetween(H, 180, 240))
        {
            g = X;
            b = C;
        }
        else if (isBetween(H, 240, 300))
        {
            r = X;
            b = C;
        }
        else if (isBetween(H, 300, 360))
        {
            r = C;
            b = X;
        }

        SDL_Color reVal;
        reVal.r = (r + m) * 255;
        reVal.g = (g + m) * 255;
        reVal.b = (b + m) * 255;
        reVal.a = 255;

        return reVal;
    }

    SDL_Color blendColors(SDL_Color A, SDL_Color B, double lambda)
    {
        if (!isBetween(lambda, 0., 1.))
        {
            throw InvalidRangeError(THROWTEXT("Mixing ratio must be in the interval [0.0, 1.0], but was "s + std::to_string(lambda)));
        }

        SDL_Color reVal;
        auto coLambda = 1-lambda;

        reVal.r = B.r * lambda + coLambda * A.r;
        reVal.g = B.g * lambda + coLambda * A.g;
        reVal.b = B.b * lambda + coLambda * A.b;
        reVal.a = B.a * lambda + coLambda * A.a;

        return reVal;
    }

// ========================================================================== //
// trigonometry

    double valuesSin[360], valuesCos[360];

    void initValuesTrigonometry()
    {
        for (auto alpha = 0; alpha < 360; ++alpha)
        {
            valuesSin[alpha] = std::sin(deg2rad(alpha));
            valuesCos[alpha] = std::cos(deg2rad(alpha));
        }
    }

// ========================================================================== //
// fonts

    std::unordered_map<std::string, TTF_Font*> fonts;

    void loadFont(const std::string& alias, const std::string& filename, int size)
    {
        auto fontPtr = TTF_OpenFont(filename.c_str(), size);

        CHECK_FILE_EXISTS(filename);

        if (fonts.contains(alias))
        {
            throw MemoryManagementError("Font alias '"s + alias + "' is already in use");
        }

        if (!fontPtr)
        {
            throw SdlInternalError("Could not load font '"s + filename + "'");
        }

        auto [it, success] = fonts.emplace(alias, fontPtr);

        if (!success)
        {
            TTF_CloseFont(fontPtr);
            throw MemoryManagementError("FontMap could not be updated");
        }
    }

// ========================================================================== //
// user defined events

    Uint32 SDL_UserEventID = 0;

    void pushUserEvent(Sint32 eventCode, void* eventData)
    {
        SDL_Event event;
        SDL_memset(&event, 0, sizeof(event));
        event.type = SDL_UserEventID;
        event.user.code = eventCode;
        event.user.data1 = eventData;
        event.user.data2 = nullptr;
        auto success = SDL_PushEvent(&event);

        if (success < 0)
        {
            throw SdlInternalError(THROWTEXT(
                                       "  Could not push user defined event\n"s +
                                       "    Event Code: " + std::to_string(eventCode) +
                                       SDL_GetError()
                                   ));
        }
    }

// ========================================================================== //
// SDL helper funcs

    SDL_Color getPixelFromSurface(SDL_Surface* surface, int x, int y)
    {
        // adopted from https://stackoverflow.com/questions/53033971/how-to-get-the-color-of-a-specific-pixel-from-sdl-surface

        int       bytesPerPixel = surface->format->BytesPerPixel;
        Uint8*    pixelAddress = static_cast<Uint8*>(surface->pixels) + y * surface->pitch + x * bytesPerPixel;
        Uint32    pixelColorPack;
        SDL_Color pixelColorStruct;

        switch (bytesPerPixel)
        {
            case 1:
                pixelColorPack = *pixelAddress;
                break;

            case 2:
                pixelColorPack = *reinterpret_cast<Uint16*>(pixelAddress);
                break;

            case 3:
                if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                {
                    pixelColorPack = pixelAddress[0] << 16 | pixelAddress[1] << 8 | pixelAddress[2];
                }
                else
                {
                    pixelColorPack = pixelAddress[0] | pixelAddress[1] << 8 | pixelAddress[2] << 16;
                }
                break;

            [[likely]] case 4:
                pixelColorPack = *(Uint32*)pixelAddress;
                break;

            default:
                pixelColorPack = 0;       /* shouldn't happen, but avoids warnings */
        }

        SDL_GetRGBA(pixelColorPack, surface->format,
                    &pixelColorStruct.r,
                    &pixelColorStruct.g,
                    &pixelColorStruct.b,
                    &pixelColorStruct.a);

        return pixelColorStruct;
    }

// ========================================================================== //
// namespace
}
