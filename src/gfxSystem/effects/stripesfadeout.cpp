// ========================================================================== //
// Depenencies

// STL
#include <iostream>

// own
#include "../window.hpp"
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

// .......................................................................... //

    size_t StripesFadeout::getNStripes() const
    {
        return nStripes;
    }

    void StripesFadeout::setNStripes(size_t newNStripes)
    {
        nStripes = newNStripes;
    }

// -------------------------------------------------------------------------- //

    void insertSplitPoints(std::vector<int>& splitPoints, std::vector<int>& basePoints, size_t nStripes, int length, int coLength)
    {
        for (auto i = 0u; i <= nStripes; ++i)
        {
            splitPoints.push_back( static_cast<double>(length) * i / nStripes );
            basePoints .push_back( (i&1) * coLength );                                    // even/odd determines from where to come in
        }
    }

    void StripesFadeout::computeSplitPoints(const Window& win)
    {
        splitPointsX.clear();
        splitPointsY.clear();

        auto [width, height] = win.getDimension();

        switch (orientation)
        {
            case Orientation::Horizontal :
                insertSplitPoints(splitPointsY, splitPointsX, nStripes, height, width);
                break;

            case Orientation::Vertical :
                insertSplitPoints(splitPointsX, splitPointsY, nStripes, width, height);
                break;
        }
    }

// ========================================================================== //
// visualEffect interface

    void StripesFadeout::install(Window& win)
    {
        computeSplitPoints(win);
        VisualEffect::install(win);

        switch (fadeoutType)
        {
            case FadeoutType::Contra:
                win.setIdleHandler(renderStripesContra);
                break;
            case FadeoutType::CloseCenter:
                break;
            case FadeoutType::Random:
                break;
        }
    }

// ========================================================================== //
// renderers

    SDL_Color blendColors(SDL_Color A, SDL_Color B, double lambda)
    {
        SDL_Color reVal;

        reVal.r = B.r * lambda + (1-lambda) * A.r;
        reVal.g = B.g * lambda + (1-lambda) * A.g;
        reVal.b = B.b * lambda + (1-lambda) * A.b;
        reVal.a = B.a * lambda + (1-lambda) * A.a;

        return reVal;
    }

// .......................................................................... //

    std::tuple<int, int, int, int> getOrientedMeasures (const StripesFadeout::Orientation& orientation, const int width, const int height)
    {
        int length, coLength, dx, dy;
        switch (orientation)
        {
            case StripesFadeout::Orientation::Horizontal :
                length = width;
                coLength = height;
                dx = 1;
                dy = 0;
                break;

            case StripesFadeout::Orientation::Vertical :
                length = height;
                coLength = width;
                dx = 0;
                dy = 1;
                break;
        }

        return std::make_tuple(length, coLength, dx, dy);
    }
// -------------------------------------------------------------------------- //

    void StripesFadeout::renderStripesContra(void* userDataPointer)
    {
        auto [userData, win, self] = unpackUserdataPointer<StripesFadeout>(userDataPointer);
        const auto [width, height] = win.getDimension();

        self.renderStoredState();

        auto color = blendColors(self.colorInitial, self.colorFinal, userData.progress);

        auto [length, coLength, dx, dy] = getOrientedMeasures(self.orientation, width, height);
//        switch (self.orientation)
//        {
//            case Orientation::Horizontal :
//                length = width;
//                coLength = height;
//                dx = 1;
//                dy = 0;
//                break;

//            case Orientation::Vertical :
//                length = height;
//                coLength = width;
//                dx = 0;
//                dy = 1;
//                break;
//        }

        for (auto i = 0u; i < self.nStripes; ++i)
        {
            auto x = self.splitPointsX[i];
            auto y = self.splitPointsY[i];

            /* if (even stripe) :
             *     endCoordinate =  animationProgress * length
             * else :
             *     endCoordinate = -animationProgress * length
             */
            const auto parity = i & 1;
            const int  sign[2] = {1, -1};
            const auto factor = sign[parity] * userData.progress;

            int endCoordinate = factor * length;

            auto w = dx * endCoordinate + dy * coLength / self.nStripes;
            auto h = dy * endCoordinate + dx * coLength / self.nStripes;

            win.fbox(x, y, w, h, color);
        }

        self.progress();
    }

// -------------------------------------------------------------------------- //

    void StripesFadeout::renderStripesCloseCenter(void* userDataPointer)
    {
        auto [userData, win, self] = unpackUserdataPointer<StripesFadeout>(userDataPointer);
        const auto [width, height] = win.getDimension();

        self.renderStoredState();

        self.progress();
    }
}
