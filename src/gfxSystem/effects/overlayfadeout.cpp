// ========================================================================== //
// Depenencies

// STL
#include <iostream>

// own
#include "../window.hpp"
#include "visualeffect.hpp"
#include "overlayfadeout.hpp"

// ========================================================================== //
// local macro

// ========================================================================== //
// namespace

namespace RetrogameBase
{

// ========================================================================== //
// CTor, DTor

    OverlayFadeout::OverlayFadeout(
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
// Getters

    const SDL_Color& OverlayFadeout::getColor() const
    {
        return color;
    }

// ========================================================================== //
// visualEffect interface

    void OverlayFadeout::install(Window& win)
    {
        VisualEffect::install(win);

        switch (fadeoutType)
        {
            case FadeoutType::Stripes:
                win.setIdleHandler(render_stripes);
                break;
            case FadeoutType::Pixelate:
                win.setIdleHandler(render_desaturate);
                break;
            case FadeoutType::Desaturate:
                win.setIdleHandler(render_desaturate);
                break;
        }
    }

// ========================================================================== //
// fadeout rendering

    void OverlayFadeout::render_stripes(void* userData)
    {
        UserData& userDataStruct = *reinterpret_cast<UserData*>(userData);

        auto& win = *userDataStruct.window;
        auto& self = *reinterpret_cast<OverlayFadeout*>(userDataStruct.effectInstanceData);

        auto [width, height] = win.getDimension();
        SDL_Color color = self.getColor();

        int currentWidth = width * userDataStruct.progress;

        self.renderStoredState();
        win.fbox(0, 0, currentWidth, height, color);

        self.progress();
    }

    void OverlayFadeout::render_desaturate(void* userData)
    {
        UserData& userDataStruct = *reinterpret_cast<UserData*>(userData);

        auto& win  = *userDataStruct.window;
        auto& self = *reinterpret_cast<OverlayFadeout*>(userDataStruct.effectInstanceData);

        auto [width, height] = win.getDimension();

        SDL_Color color = self.getColor();
        color.a = userDataStruct.progress * 255;

        self.renderStoredState();
        win.fbox(0, 0, width, height, color);

        self.progress();
    }
}
