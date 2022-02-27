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
                win.setIdleHandler(render_fadeout);
                break;
            case FadeoutType::Pixelate:
                win.setIdleHandler(render_fadeout);
                break;
            case FadeoutType::Desaturate:
                win.setIdleHandler(render_fadeout);
                break;
        }
    }

// ========================================================================== //
// fadeout rendering

    void OverlayFadeout::render_fadeout(void* userData)
    {
        UserData& userDataStruct = *reinterpret_cast<UserData*>(userData);
        auto& win = *userDataStruct.window;
        auto [width, height] = userDataStruct.window->getDimension();

        SDL_Color color = reinterpret_cast<OverlayFadeout*>(userDataStruct.effectInstanceData)->getColor();
        color.a = userDataStruct.progress * 255;

        win.clear(color_black);

        SDL_RenderCopy(userDataStruct.windowRenderer,
                       userDataStruct.windowTexture,
                       nullptr, nullptr);

        win.fbox(0, 0, width, height, color);

        SDL_UpdateWindowSurface(userDataStruct.sdlWindow);

        userDataStruct.effectInstanceData->progress();
    }
}
