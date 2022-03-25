#ifndef STRIPESFADEOUT_HPP
#define STRIPESFADEOUT_HPP

// ========================================================================== //
// Depenencies

// STL

// SDL

// own
#include "../../base/globals.hpp"
#include "visualeffect.hpp"

// ========================================================================== //
// Class

namespace RetrogameBase
{
    class StripesFadeout : public VisualEffect
    {
        public:
            enum class FadeoutType
            {
                Contra,
                CloseCenter,
                Random
            };

            enum class Orientation
            {
                Horizontal,
                Vertical
            };

        private:
            FadeoutType fadeoutType;
            Orientation orientation;

            SDL_Color colorInitial = color_transparent;
            SDL_Color colorFinal = color_black;

        protected:
            virtual void install(Window& win);

        public:
            // -------------------------------------------------------------- //
            // CTor, DTor

            StripesFadeout() = delete;
            StripesFadeout(const FadeoutType fadeoutType, const Orientation orientation, double milliseconds, double fps);

            // -------------------------------------------------------------- //
            // getters, setters

            FadeoutType getFadeoutType() const;
            void setFadeoutType(FadeoutType newFadeoutType);

            Orientation getOrientation() const;
            void setOrientation(Orientation newOrientation);

            const SDL_Color& getColorInitial() const;
            void setColorInitial(const SDL_Color& newColorInitial);

            const SDL_Color& getColorFinal() const;
            void setColorFinal(const SDL_Color& newColorFinal);

            // -------------------------------------------------------------- //
            // renderers

            static void renderStripesContra(void* userDataPointer);
            static void renderStripesCloseCenter(void* userDataPointer);
            static void renderStripesRandom(void* userDataPointer);
    };
}

#endif // STRIPESFADEOUT_HPP
