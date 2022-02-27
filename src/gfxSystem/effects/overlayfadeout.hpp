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
#include "../../base/globals.hpp"
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
            const SDL_Color     color;

        protected:
            virtual void install(Window& win);

        public:
            OverlayFadeout() = delete;
            OverlayFadeout(const FadeoutType fadeoutType, double milliseconds, double fps, const SDL_Color color = color_black);

            const SDL_Color& getColor() const;

            static void render_fadeout(void* userData);
    };
}

#endif // OVERLAYFADEOUT_HPP
