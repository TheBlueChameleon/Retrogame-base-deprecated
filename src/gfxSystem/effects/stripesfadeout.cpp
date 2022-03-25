#include "stripesfadeout.hpp"

namespace RetrogameBase
{
    StripesFadeout::FadeoutType StripesFadeout::getFadeoutType() const
    {
        return fadeoutType;
    }

    void StripesFadeout::setFadeoutType(FadeoutType newFadeoutType)
    {
        fadeoutType = newFadeoutType;
    }

    StripesFadeout::Orientation StripesFadeout::getOrientation() const
    {
        return orientation;
    }

    void StripesFadeout::setOrientation(Orientation newOrientation)
    {
        orientation = newOrientation;
    }

    const SDL_Color& StripesFadeout::getColorInitial() const
    {
        return colorInitial;
    }

    void StripesFadeout::setColorInitial(const SDL_Color& newColorInitial)
    {
        colorInitial = newColorInitial;
    }

    const SDL_Color& StripesFadeout::getColorFinal() const
    {
        return colorFinal;
    }

    void StripesFadeout::setColorFinal(const SDL_Color& newColorFinal)
    {
        colorFinal = newColorFinal;
    }
}
