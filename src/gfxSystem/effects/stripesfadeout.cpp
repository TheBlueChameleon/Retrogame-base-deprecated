// ========================================================================== //
// Depenencies

// STL

// own
#include "visualeffect.hpp"
#include "stripesfadeout.hpp"

// ========================================================================== //
// local macro

#define THROWTEXT(msg) ("RUNTIME EXCEPTION IN "s + (__PRETTY_FUNCTION__) + "\n"s + msg)

// ========================================================================== //
// namespace

namespace RetrogameBase
{

// ========================================================================== //
// CTor, DTor

    StripesFadeout::StripesFadeout(const FadeoutType fadeoutType, const Orientation orientation, double milliseconds, double fps) :
        VisualEffect(fps, fps * milliseconds / 1000.),
        fadeoutType(fadeoutType),
        orientation(orientation)
    {}

// ========================================================================== //
// Getters, Setters

    StripesFadeout::FadeoutType StripesFadeout::getFadeoutType() const
    {
        return fadeoutType;
    }

    void StripesFadeout::setFadeoutType(FadeoutType newFadeoutType)
    {
        fadeoutType = newFadeoutType;
    }

// .......................................................................... //

    StripesFadeout::Orientation StripesFadeout::getOrientation() const
    {
        return orientation;
    }

    void StripesFadeout::setOrientation(Orientation newOrientation)
    {
        orientation = newOrientation;
    }

// .......................................................................... //

    const SDL_Color& StripesFadeout::getColorInitial() const
    {
        return colorInitial;
    }

    void StripesFadeout::setColorInitial(const SDL_Color& newColorInitial)
    {
        colorInitial = newColorInitial;
    }

// .......................................................................... //

    const SDL_Color& StripesFadeout::getColorFinal() const
    {
        return colorFinal;
    }

    void StripesFadeout::setColorFinal(const SDL_Color& newColorFinal)
    {
        colorFinal = newColorFinal;
    }

// ========================================================================== //
// visualEffect interface

// ========================================================================== //
// renderers

// .......................................................................... //
}
