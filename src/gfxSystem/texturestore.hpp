#ifndef TEXTURESTORE_HPP
#define TEXTURESTORE_HPP

// ========================================================================== //
// Depenencies

// STL
#include <stdexcept>

#include <vector>
#include <string>

// SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// own
#include "window.hpp"

// ========================================================================== //
// Class

namespace RetrogameBase {
    class TextureStore {
        private:
            const Window* win;

            std::vector<SDL_Texture*>        textures;
            std::vector<std::string>         filenames;
            std::vector<std::pair<int, int>> dimensions;

        public:
            static const size_t NOINDEX = std::numeric_limits<size_t>::max();

            // -------------------------------------------------------------- //
            // CTor, DTor
            TextureStore() = delete;
            TextureStore(const Window* win);
            ~TextureStore();

            // -------------------------------------------------------------- //
            // getters

            const Window& getWin () const;

            size_t size() const;

            SDL_Texture*        getTexture(size_t ID) const;
            const std::string&  getFilename(const int ID) const;
            std::pair<int, int> getImageDimensions(const int ID) const;

            size_t findByFilename(const std::string& filename) const;         // returns NOINDEX if image not in store

            // -------------------------------------------------------------- //
            // setters/modifiers

            void reset();

            size_t addFrame (const std::string& filename);                    // returns index of loaded image in store. Prevents double loading

            // -------------------------------------------------------------- //
            // display

            void put (int ID, int x, int y);
    };

}
#endif // TEXTURESTORE_HPP
