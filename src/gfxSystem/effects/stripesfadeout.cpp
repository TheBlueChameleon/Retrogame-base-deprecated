// ========================================================================== //
// Depenencies

// STL
#include <iostream>

#include <string>
using namespace std::string_literals;

// own
#include "../../base/exceptions.hpp"
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
        if (newNStripes < 1)
        {
            throw PositiveValueExpectedError(THROWTEXT("Number of stripes needs to be positive"));
        }

        nStripes = newNStripes;
    }

// ========================================================================== //
// visualEffect interface

    void StripesFadeout::prepareInstance()
    {

        switch (fadeoutType)
        {
            case FadeoutType::Contra:
            case FadeoutType::CloseCenter:
                computeSplitPoints();
                break;

            case FadeoutType::Random:
                computeRandomPoints();
                break;
        }
    }

    void StripesFadeout::tidyUpInstance()
    {}
// .......................................................................... //

    std::function<void (void*)> StripesFadeout::getRenderer()
    {
        switch (fadeoutType)
        {
            case FadeoutType::Contra:
                return renderStripesContra;
            case FadeoutType::CloseCenter:
                return renderStripesCloseCenter;
            case FadeoutType::Random:
                return renderStripesRandom;
        }

        // cannot happen, appeases compiler
        return nullptr;
    }

// ========================================================================== //
// Helper Funcs

    void insertSplitPoints(std::vector<int>& splitPoints,
                           std::vector<int>& basePoints,
                           size_t nStripes,
                           int offset, int coOffset,
                           int length, int coLength)
    {
        for (auto i = 0u; i <= nStripes; ++i)
        {
            splitPoints.push_back(   offset + static_cast<double>(length) * i / nStripes );
            basePoints .push_back( coOffset + (i&1) * coLength );                                    // even/odd determines from where to come in
        }
    }

    void StripesFadeout::computeSplitPoints()
    {
        splitPointsX.clear();
        splitPointsY.clear();

        switch (orientation)
        {
            case Orientation::Horizontal :
                insertSplitPoints(splitPointsY, splitPointsX, nStripes,
                                  effectY, effectX,
                                  effectHeight, effectWidth);
                break;

            case Orientation::Vertical :
                insertSplitPoints(splitPointsX, splitPointsY, nStripes,
                                  effectX, effectY,
                                  effectWidth, effectHeight);
                break;
        }
    }

// .......................................................................... //

    void insertRandomPoints(std::vector<int>& random, std::vector<int>& equidistant, size_t nStripes, int length, int coLength)
    {
        const auto stripeWidth = coLength / nStripes;

        for (auto i = 0u; i < nStripes; ++i)
        {
            const double r = std::rand() / static_cast<double>(RAND_MAX);

            equidistant.push_back(i * stripeWidth);
            random     .push_back(r * length);
        }
    }


    void StripesFadeout::computeRandomPoints()
    {
        splitPointsX.clear();
        splitPointsY.clear();

        auto [width, height] = window->getDimension();

        switch (orientation)
        {
            case Orientation::Horizontal :
                insertRandomPoints(splitPointsX, splitPointsY, nStripes, width, height);
                break;

            case Orientation::Vertical :
                insertRandomPoints(splitPointsY, splitPointsX, nStripes, height, width);
                break;
        }
    }

// .......................................................................... //

    std::tuple<int, int, int, int, int, int> StripesFadeout::getOrientedMeasures() const
    {
        // returns offset, coOffset, length, coLength, dx, dy

        switch (orientation)
        {
            case StripesFadeout::Orientation::Horizontal :
                return std::make_tuple(effectX, effectY, effectWidth, effectHeight, 1, 0);

            case StripesFadeout::Orientation::Vertical :
                return std::make_tuple(effectY, effectX, effectHeight, effectWidth, 0, 1);
        }

        // this never happens but appeases the compiler:
        return std::make_tuple(0, 0, 0, 0, 0, 0);
    }

// ========================================================================== //
// renderers

    void StripesFadeout::renderStripesContra(void* instanceData)
    {
        auto [self, win] = unpackSelfAndWin<StripesFadeout>(instanceData);

        self.renderStoredState();

        const auto color = blendColors(self.colorInitial, self.colorFinal, self.progress);
        const auto [offset, coOffset, length, coLength, dx, dy] = self.getOrientedMeasures();

        const int  sign[2] = {1, -1};
        const auto stripeWidth = coLength / self.nStripes;

        for (auto i = 0u; i < self.nStripes; ++i)
        {
            const auto x = self.splitPointsX[i];
            const auto y = self.splitPointsY[i];

            // if (even stripe) :
            //      endCoordinate =  animationProgress * length
            // else :
            //      endCoordinate = -animationProgress * length

            const auto parity = i & 1;
            const auto factor = sign[parity] * self.progress;

            const int endCoordinate = factor * length;

            const auto w = dx * endCoordinate + dy * stripeWidth;
            const auto h = dy * endCoordinate + dx * stripeWidth;

            win.fbox(x, y, w, h, color);
        }

        self.advanceFrame();
    }

// .......................................................................... //

    void StripesFadeout::renderStripesCloseCenter(void* instanceData)
    {
        auto [self, win] = unpackSelfAndWin<StripesFadeout>(instanceData);

        self.renderStoredState();

        const auto color = blendColors(self.colorInitial, self.colorFinal, self.progress);
        const auto [offset, coOffset, length, coLength, dx, dy] = self.getOrientedMeasures();

        const int  sign[2] = {1, -1};
        const auto centerOffset = (self.nStripes > 1) * length * 0.2;
        const auto stripeWidth = coLength / self.nStripes;

        for (auto i = 0u; i < self.nStripes; ++i)
        {
            const auto parity = i & 1;
            const auto factor = sign[parity] * self.progress;
            const auto center = (length >> 1) - centerOffset;

            const auto x1 = self.splitPointsX[i];
            const auto y1 = self.splitPointsY[i];
            const auto x2 = dy * x1   +   dx * (length * !parity +   offset);
            const auto y2 = dx * y1   +   dy * (length * !parity + coOffset);

            const int w1 =  dx * factor *  center             +   dy * stripeWidth;
            const int w2 = -dx * factor * (length - center)   +   dy * stripeWidth;
            const int h1 =  dy * factor *  center             +   dx * stripeWidth;
            const int h2 = -dy * factor * (length - center)   +   dx * stripeWidth;

            win.fbox(x1, y1, w1, h1, color);
            win.fbox(x2, y2, w2, h2, color);
        }

        self.advanceFrame();
    }

// .......................................................................... //

    void StripesFadeout::renderStripesRandom(void* instanceData)
    {
        auto [self, win] = unpackSelfAndWin<StripesFadeout>(instanceData);

        self.renderStoredState();

        const auto color = blendColors(self.colorInitial, self.colorFinal, self.progress);
        const auto [offset, coOffset, length, coLength, dx, dy] = self.getOrientedMeasures();
        const auto stripeWidth = coLength / self.nStripes;

        for (auto i = 0u; i < self.nStripes; ++i)
        {
            const int x = self.splitPointsX[i] - dx * self.progress * length;
            const int y = self.splitPointsY[i] - dy * self.progress * length;

            const int w = dx * 2 * length * self.progress   +   dy * stripeWidth;
            const int h = dy * 2 * length * self.progress   +   dx * stripeWidth;

            win.fbox(x, y, w, h, color);
        }

        self.advanceFrame();
    }
    /*
            void StripesFadeout::renderStripesRandom(void* userDataPointer)
            {
                auto [userData, win, self] = unpackUserdataPointer<StripesFadeout>(userDataPointer);
                const auto [width, height] = win.getDimension();

                self.renderStoredState();

                const auto color = blendColors(self.colorInitial, self.colorFinal, userData.progress);
                const auto [length, coLength, dx, dy] = getOrientedMeasures(self.orientation, width, height);
                const auto stripeWidth = coLength / self.nStripes;

                for (auto i = 0u; i < self.nStripes; ++i)
                {
                    const int x = self.splitPointsX[i] - dx * userData.progress * length;
                    const int y = self.splitPointsY[i] - dy * userData.progress * length;

                    const int w = dx * 2 * length * userData.progress   +   dy * stripeWidth;
                    const int h = dy * 2 * length * userData.progress   +   dx * stripeWidth;

                    win.fbox(x, y, w, h, color);
                }

                self.progress();
            }
            */
}
