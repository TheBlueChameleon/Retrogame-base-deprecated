#ifndef OVERLAYFADEOUT_HPP
#define OVERLAYFADEOUT_HPP

// ========================================================================== //
// Depenencies

// STL
#include <functional>
#include <memory>

// SDL
#include <SDL2/SDL.h>

// own
#include "visualeffect.hpp"

// ========================================================================== //
// Class

namespace RetrogameBase
{
    class OverlayFadeout : public VisualEffect
    {
        public:
            enum class FadeoutType
            {
                Stripes,
                Pixelate,
                Desaturate
            };

        private:
            const FadeoutType   fadeoutType;

        protected:
            virtual void install(Window& win);

        public:
            OverlayFadeout() = delete;
            OverlayFadeout(const FadeoutType fadeoutType, size_t totalFrames, double fps);
            OverlayFadeout(const FadeoutType fadeoutType, double milliseconds, double fps);

            static void render_fadeout(void* userData);
    };
}

#endif // OVERLAYFADEOUT_HPP
