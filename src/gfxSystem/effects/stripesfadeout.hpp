/*
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

            size_t nStripes = 1;

            std::vector<int> splitPointsX;
            std::vector<int> splitPointsY;

            void computeSplitPoints (const UserData& userData);
            void computeRandomPoints(const UserData& userData);

        protected:
            virtual void prepareInstance(UserData& userData);
            virtual std::function<void (void*)> getRenderer();

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

            size_t getNStripes() const;
            void setNStripes(size_t newNStripes);

            // -------------------------------------------------------------- //
            // renderers

            static void renderStripesContra(void* userDataPointer);
            static void renderStripesCloseCenter(void* userDataPointer);
            static void renderStripesRandom(void* userDataPointer);
    };
}

#endif // STRIPESFADEOUT_HPP
*/
