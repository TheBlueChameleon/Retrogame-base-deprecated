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

// own
#include "../xmlSystem/xmlwrapper.hpp"

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
            Animation(Window& window, const std::string& filename);

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
            void resetPhase();

            void advanceFrame();

            void addFrame (size_t ID, size_t repetitions);
            void addFrame (const std::string& filename, size_t repetitions);

            void loadXML (const std::string& filename);
        private:
            static const std::pair<std::string, int> INVALID_TAG;
            std::pair<std::string, int> getFilenameAndRepetitionFromTag(pugi::xml_node& node, const std::string& filename) const;

        public:

            // -------------------------------------------------------------- //
            // display
    };
}
#endif // ANIMATION_HPP
