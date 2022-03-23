#ifndef SIMPLEFADEOUT_HPP
#define SIMPLEFADEOUT_HPP

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
    class SimpleFadeout : public VisualEffect
    {
        public:
            enum class FadeoutType
            {
                Blur,
                Pixelate,
                Desaturate
            };

        private:
            FadeoutType   fadeoutType;
            SDL_Color     color;

        protected:
            virtual void install(Window& win);

        public:
            SimpleFadeout() = delete;
            SimpleFadeout(const FadeoutType fadeoutType, double milliseconds, double fps, const SDL_Color color = color_black);

            const SDL_Color& getColor() const;
            void setColor(const SDL_Color& newColor);

            FadeoutType getFadeoutType() const;
            void setFadeoutType(FadeoutType newFadeoutType);

            static void render_stripes(void* userData);

            static void render_blur(void* userData);
            static void render_pixelate(void* userData);
            static void render_desaturate(void* userData);
    };
}

#endif // SIMPLEFADEOUT_HPP
