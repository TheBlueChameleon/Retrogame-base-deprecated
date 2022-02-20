#ifndef ANIMATIONLAYERSTORE_HPP
#define ANIMATIONLAYERSTORE_HPP

// ========================================================================== //
// Depenencies

// STL

// own
#include "animationlayer.hpp"

// ========================================================================== //
// Class

namespace RetrogameBase
{
    class Window;

    class AnimationLayerStore
    {
        private:
            Window& window;

            std::vector<AnimationLayer> animationLayers;

        public:
            friend class Window;

            // ------------------------------------------------------------------ //
            // CTor, DTor

            AnimationLayerStore() = delete;
            AnimationLayerStore(RetrogameBase::Window& window);

            // -------------------------------------------------------------- //
            // getters

            size_t size() const;

            AnimationLayer& getLayer(size_t ID);

            // -------------------------------------------------------------- //
            // setters/modifiers

            void reset();
        private:
            void reset_private();

        public:
            std::pair<size_t, AnimationLayer&> addLayer();
            std::pair<size_t, AnimationLayer&> addLayer(const std::string& filename);

    };
}
#endif // ANIMATIONLAYERSTORE_HPP
