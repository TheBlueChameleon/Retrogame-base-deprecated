#ifndef SIMPLEFADEOUT_HPP
#define SIMPLEFADEOUT_HPP

// ========================================================================== //
// Depenencies

// STL
#include <span>
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

            std::vector<Uint16>            buffer;
            std::vector<std::span<Uint16>> bufferViews;
            std::vector<std::span<Uint8>>  surfaceViews;

            void prepareInstanceBlur();
            void tidyUpInstanceBlur ();

        protected:
            virtual std::function<void (void*)> getRenderer();
            virtual void prepareInstance();
            virtual void tidyUpInstance ();

        public:
            // -------------------------------------------------------------- //
            // CTor, DTor

            SimpleFadeout() = delete;
            SimpleFadeout(const FadeoutType fadeoutType, double milliseconds, double fps, const SDL_Color color = color_black);

            // -------------------------------------------------------------- //
            // getters, setters

            virtual const SDL_Color& getColor() const;
            void setColor(const SDL_Color& newColor);

            FadeoutType getFadeoutType() const;
            void setFadeoutType(FadeoutType newFadeoutType);

            // -------------------------------------------------------------- //
            // renderers

            static void renderBlur(void* instanceData);
            static void renderPixelate(void* instanceData);
            static void renderDesaturate(void* instanceData);
    };
}

#endif // SIMPLEFADEOUT_HPP
