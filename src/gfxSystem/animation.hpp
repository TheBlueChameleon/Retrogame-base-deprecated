#ifndef ANIMATION_HPP
#define ANIMATION_HPP

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
    class TextureStore;

    class Animation
    {
        private:
            Window& window;
            TextureStore& textureStore;

            std::vector<size_t> frames;
            size_t currentPhase = 0;

            std::pair<int, int> dimension = {-1, -1};

        public:
            static constexpr std::pair<int, int> NODIMENSION = {-1, -1};

            // -------------------------------------------------------------- //
            // CTor, DTor

            Animation() = delete;
            Animation(Window& window);

            // -------------------------------------------------------------- //
            // getters

            size_t size() const;

            const std::pair<int, int>& getDimension() const;
            const std::vector<size_t>& getFrameIDs() const;

            size_t       getCurrentPhase() const;
            size_t       getCurrentFrameID() const;
            SDL_Texture* getCurrentFrameTexture() const;

            // -------------------------------------------------------------- //
            // setters/modifiers

            void reset();

            void advanceFrame();

            void addFrame (size_t ID);
            void addFrame (const std::string& filename);

            void loadXML (const std::string& filename);

            // -------------------------------------------------------------- //
            // display

    };
}
#endif // ANIMATION_HPP
