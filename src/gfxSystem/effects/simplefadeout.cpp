// ========================================================================== //
// Depenencies

// STL
#include <iostream>

// own
#include "../window.hpp"
#include "visualeffect.hpp"
#include "simplefadeout.hpp"

// ========================================================================== //
// local macro

// ========================================================================== //
// namespace

namespace RetrogameBase
{

// ========================================================================== //
// CTor, DTor

    SimpleFadeout::SimpleFadeout(
        const FadeoutType fadeoutType,
        double milliseconds,
        double fps,
        const SDL_Color color
    ) :
        VisualEffect(fps, fps * milliseconds / 1000.),
        fadeoutType(fadeoutType),
        color(color)
    {}

// ========================================================================== //
// Getters, Setters

    const SDL_Color& SimpleFadeout::getColor() const
    {
        return color;
    }

    void SimpleFadeout::setColor(const SDL_Color& newColor)
    {
        color = newColor;
    }

    SimpleFadeout::FadeoutType SimpleFadeout::getFadeoutType() const
    {
        return fadeoutType;
    }

    void SimpleFadeout::setFadeoutType(FadeoutType newFadeoutType)
    {
        fadeoutType = newFadeoutType;
    }

    double SimpleFadeout::getDuration() const
    {
        return totalFrames / fps * 1000;
    }

    void SimpleFadeout::setDuration(const double milliseconds)
    {
        this->setTotalFrames(fps * milliseconds / 1000.);
    }

// ========================================================================== //
// visualEffect interface

    void SimpleFadeout::install(Window& win)
    {
        VisualEffect::install(win);

        switch (fadeoutType)
        {
            case FadeoutType::Stripes:
                win.setIdleHandler(render_stripes);
                break;
            case FadeoutType::Pixelate:
                win.setIdleHandler(render_pixelate);
                break;
            case FadeoutType::Desaturate:
                win.setIdleHandler(render_desaturate);
                break;
        }
    }

// ========================================================================== //
// fadeout rendering

    void SimpleFadeout::render_stripes(void* userData)
    {
        UserData& userDataStruct = *reinterpret_cast<UserData*>(userData);

        auto& win = *userDataStruct.window;
        auto& self = *reinterpret_cast<SimpleFadeout*>(userDataStruct.effectInstanceData);

        const auto [width, height] = win.getDimension();
        SDL_Color color = self.getColor();

        int currentWidth = width * userDataStruct.progress;

        self.renderStoredState();
        win.fbox(0, 0, currentWidth, height, color);

        self.progress();
    }

    SDL_Color getAverageColor(SDL_Surface* surface, const int startX, const int startY, const int width, const int height)
    {
        long long r = 0, g = 0, b = 0;
        const long long area = width * height;

        for     (auto x=startX; x < startX+width ; ++x)
        {
            for (auto y=startY; y < startY+height; ++y)
            {
                const auto thisPixel = getPixelFromSurface(surface, x, y);

                r += thisPixel.r;
                g += thisPixel.g;
                b += thisPixel.b;
            }
        }

        SDL_Color reVal;
        reVal.r = r / area;
        reVal.g = g / area;
        reVal.b = b / area;
        reVal.a = 255;

        return reVal;
    };

    void SimpleFadeout::render_pixelate(void* userData)
    {
        UserData& userDataStruct = *reinterpret_cast<UserData*>(userData);

        auto& win  = *userDataStruct.window;
        auto& self = *reinterpret_cast<SimpleFadeout*>(userDataStruct.effectInstanceData);

        const auto [width, height]  = win.getDimension();
        const int pixelWidth  = width  * userDataStruct.progress + 1;
        const int pixelHeight = height * userDataStruct.progress + 1;

        self.renderStoredState();

        for     (auto x=0u; x < width ; x+= pixelWidth)
        {
            for (auto y=0u; y < height; y+= pixelHeight)
            {
                auto color = getAverageColor(userDataStruct.windowSurface, x, y, pixelWidth, pixelHeight);
                win.fbox(x, y, pixelWidth, pixelHeight, color);
            }
        }

        auto color = getAverageColor(userDataStruct.windowSurface, 10, 10, pixelWidth, pixelHeight);

        self.progress();
    }

    void SimpleFadeout::render_desaturate(void* userData)
    {
        UserData& userDataStruct = *reinterpret_cast<UserData*>(userData);

        auto& win  = *userDataStruct.window;
        auto& self = *reinterpret_cast<SimpleFadeout*>(userDataStruct.effectInstanceData);

        const auto [width, height] = win.getDimension();

        SDL_Color color = self.getColor();
        color.a = userDataStruct.progress * 255;

        self.renderStoredState();
        win.fbox(0, 0, width, height, color);

        self.progress();
    }
}
