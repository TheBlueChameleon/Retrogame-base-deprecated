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
#include <algorithm>
#include <cctype>
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
// fonts

    std::unordered_map<std::string, TTF_Font*> fonts;

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
    }

    void initGlobals()
    {
        loadFont("fixed-small","../font/FreeMono.ttf", 8);
        loadFont("fixed-medium","../font/FreeMono.ttf", 16);
        loadFont("fixed-big","../font/FreeMono.ttf", 24);

        std::atexit(freeGlobals);
    }

    void freeGlobals()
    {
        for (auto & [name, fontPtr] : fonts)
        {
            TTF_CloseFont(fontPtr);
        }
    }

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
// namespace
}
