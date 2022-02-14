#ifndef ANIMATIONSTORE_HPP
#define ANIMATIONSTORE_HPP

// ========================================================================== //
// Depenencies

// STL
#include <stdexcept>

#include <vector>
#include <string>

#include "animation.hpp"

// ========================================================================== //
// Class

namespace RetrogameBase
{
    class Window;
    class TextureStore;
    class Animation;

    class AnimationStore
    {
        private:
            Window& window;
            TextureStore& textureStore;

            std::vector<std::string> filenames;
            std::vector<Animation>   animations;

            void reset_private();

        public:
            static const size_t NOINDEX = std::numeric_limits<size_t>::max();
            friend class Window;

            // -------------------------------------------------------------- //
            // CTor, DTor

            AnimationStore(Window& window);

            // -------------------------------------------------------------- //
            // getters

            const Window&       getWindow () const;
            const TextureStore& getTextureStore () const;

            size_t size() const;

            Animation& getAnimation(size_t ID) const;
            const std::string&  getFilename(const int ID) const;
            std::pair<int, int> getImageDimension(const int ID) const;

            size_t findByFilename(const std::string& filename) const;         // returns NOINDEX if image not in store

            // -------------------------------------------------------------- //
            // setters/modifiers

            void reset();

            size_t addAnimation (const std::string& filename);                    // returns index of loaded image in store. Prevents double loading

            void advanceAll();

    };
}

#endif // ANIMATIONSTORE_HPP
