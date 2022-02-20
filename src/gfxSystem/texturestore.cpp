// ========================================================================== //
// Depenencies

// STL
#include <string>
using namespace std::string_literals;

#include <filesystem>
namespace fs = std::filesystem;

// SDL
#include <SDL2/SDL_image.h>

// own
#include "../base/globals.hpp"
#include "window.hpp"
#include "texturestore.hpp"

// ========================================================================== //
// local macro

#define THROWTEXT(msg) ("RUNTIME EXCEPTION IN "s + (__PRETTY_FUNCTION__) + "\n"s + msg)

#ifdef DEBUG
#define CHECK_GFX_INDEX(ID) { \
        if ( (ID >= this->size()) ) { \
            throw std::out_of_range(THROWTEXT( "  Invalid GfxStore ID: "s + std::to_string(ID) )); \
        } \
    }
#else
#define CHECK_GFX_INDEX(ID) {}
#endif

#define CHECK_FILE_EXISTS(filename) {\
        if (!fs::exists(filename)) { \
            throw FileNotFoundError(THROWTEXT( \
                                               "  file not found: '"s + filename + "'" \
                                             )); \
        } \
    }

// ========================================================================== //
// namespace

namespace RetrogameBase
{

// ========================================================================== //
// CTor, DTor

    TextureStore::TextureStore(Window& window) :
        window(window) {}

    TextureStore::~TextureStore()
    {
        reset_private();
    }

// ========================================================================== //
// getters

    const Window& TextureStore::getWindow() const
    {
        return window;
    }

    size_t TextureStore::size() const
    {
        return textures.size();
    }

    size_t TextureStore::getTotalMemory() const
    {
        return totalMemory;
    }

    SDL_Texture* TextureStore::getTexture(size_t ID) const
    {
        CHECK_GFX_INDEX(ID);
        return textures[ID];
    }

    const std::string& TextureStore::getFilename(const int ID) const
    {
        CHECK_GFX_INDEX(ID);
        return filenames[ID];
    }

    std::pair<int, int> TextureStore::getTextureDimension(const int ID) const
    {
        CHECK_GFX_INDEX(ID);
        return dimensions[ID];
    }

    size_t TextureStore::getTextureMemorySize(const int ID) const
    {
        CHECK_GFX_INDEX(ID);
        int w, h;
        Uint32 format;

        SDL_QueryTexture(textures[ID], &format, NULL, &w, &h);

        return w * h * SDL_BYTESPERPIXEL(format);
    }

    size_t TextureStore::findByFilename(const std::string& filename) const
    {
        auto searchIterator = std::find(filenames.begin(),
                                        filenames.end(),
                                        filename);
        if (searchIterator != filenames.end())
        {
            return std::distance(filenames.begin(), searchIterator);
        }
        else
        {
            return NOINDEX;
        }
    }

// ========================================================================== //
// setters/modifiers

    void TextureStore::reset()
    {
        window.resetStores(Window::ResetStoresDepth::Textures);
    }

    size_t TextureStore::addFrame(const std::string& filename)
    {
        CHECK_FILE_EXISTS(filename);

        auto index = findByFilename(filename);

        if (index != NOINDEX)
        {
            return index;
        }

        SDL_Surface* loadedSurface = IMG_Load( filename.c_str() );

        if( loadedSurface == NULL )
        {
            throw SdlInternalError(THROWTEXT(
                                       "Unable to load image " + filename + "!\n"
                                       "\tSDL_image Error: " + IMG_GetError()
                                   ));
        }

        SDL_Texture* newTexture = SDL_CreateTextureFromSurface(window.getRenderer(),
                                  loadedSurface);
        if(!newTexture)
        {
            throw SdlInternalError(THROWTEXT(
                                       "Unable to create texture from image " + filename + "!\n"
                                       "\tSDL_image Error: " + IMG_GetError()
                                   ));
        }

        Uint32 format;
        SDL_QueryTexture(newTexture, &format, nullptr, nullptr, nullptr);
        totalMemory += loadedSurface->w * loadedSurface->h * SDL_BYTESPERPIXEL(format);

        filenames.push_back(filename);
        textures.push_back(newTexture);
        dimensions.push_back( std::make_pair(loadedSurface->w, loadedSurface->h) );

        SDL_FreeSurface( loadedSurface );

        return textures.size() - 1;
    }

    void TextureStore::reset_private()
    {
        for (auto& tex : textures)
        {
            SDL_DestroyTexture(tex);
        }

        totalMemory = 0;

        textures.clear();
        filenames.clear();
        dimensions.clear();
    }

// ========================================================================== //
// display

    void TextureStore::put(int ID, int x, int y)
    {
        CHECK_GFX_INDEX(ID);

        SDL_Rect dest = {x, y, dimensions[ID].first, dimensions[ID].second};

        SDL_RenderCopy(window.getRenderer(), textures[ID], NULL, &dest);
    }

    void TextureStore::put(int ID, int x, int y, int angle)
    {
        CHECK_GFX_INDEX(ID);

        SDL_Rect dest = {x, y, dimensions[ID].first, dimensions[ID].second};

        SDL_RenderCopyEx(window.getRenderer(),
                         textures[ID],
                         NULL,                          // &srcRect
                         &dest,
                         angle,
                         NULL,                          // SDL_FPoint *center
                         SDL_FLIP_NONE                  // flip
                        );
    }

// ========================================================================== //
// namespace

}
