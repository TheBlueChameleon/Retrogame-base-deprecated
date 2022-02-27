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

    OverlayFadeout::OverlayFadeout(const FadeoutType fadeoutType, double milliseconds, double fps) :
        VisualEffect(fps, fps * milliseconds / 1000.),
        fadeoutType(fadeoutType)
    {}

    OverlayFadeout::OverlayFadeout(const FadeoutType fadeoutType, size_t totalFrames, double fps) :
        VisualEffect(fps, totalFrames),
        fadeoutType(fadeoutType)
    {}

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

        userDataStruct.window->clear(color_black);

        SDL_RenderCopy(userDataStruct.windowRenderer,
                       userDataStruct.windowTexture,
                       nullptr, nullptr);

        Uint8 alpha = userDataStruct.progress * 255;
        SDL_Color color = {255,0,255, alpha};
        userDataStruct.window->fbox(10, 10, 400, 300, color);

        SDL_UpdateWindowSurface(userDataStruct.sdlWindow);

        userDataStruct.effectInstanceData->progress();
    }


}
