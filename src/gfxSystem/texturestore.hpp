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

// ========================================================================== //
// Class

namespace RetrogameBase
{
    class Window;

    class TextureStore
    {
        private:
            Window& window;

            std::vector<SDL_Texture*>        textures;
            std::vector<std::string>         filenames;
            std::vector<std::pair<int, int>> dimensions;

            size_t                           totalMemory = 0;

        public:
            static const size_t NOINDEX = std::numeric_limits<size_t>::max();
            friend class Window;

            // -------------------------------------------------------------- //
            // CTor, DTor

            TextureStore() = delete;
            TextureStore(Window& window);
            ~TextureStore();

            // -------------------------------------------------------------- //
            // getters

            const Window& getWindow () const;

            size_t size() const;

            size_t getTotalMemory() const;

            SDL_Texture*        getTexture(size_t ID) const;
            const std::string&  getFilename(const int ID) const;
            std::pair<int, int> getTextureDimension(const int ID) const;
            size_t              getTextureMemorySize(const int ID) const;

            size_t findByFilename(const std::string& filename) const;         // returns NOINDEX if image not in store

            // -------------------------------------------------------------- //
            // setters/modifiers

            void reset();
        private:
            void reset_private();
        public:

            size_t addFrame (const std::string& filename);                    // returns index of loaded image in store. Prevents double loading

            // -------------------------------------------------------------- //
            // display

            void put (int ID, int x, int y);
            void put (int ID, int x, int y, int angle);

    };

}
#endif // TEXTURESTORE_HPP
