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

// ========================================================================== //
// Class

namespace RetrogameBase
{
    class Window;

    class TextureStore
    {
        private:
            Window& win;

            std::vector<SDL_Texture*>        textures;
            std::vector<std::string>         filenames;
            std::vector<std::pair<int, int>> dimensions;

            void reset_private();

        public:
            static const size_t NOINDEX = std::numeric_limits<size_t>::max();
            friend class Window;

            // -------------------------------------------------------------- //
            // CTor, DTor
            TextureStore() = delete;
            TextureStore(Window& win);
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
