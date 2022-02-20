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

    OverlayFadeout::OverlayFadeout(const FadeoutType fadeoutType, size_t milliseconds, double fps) :
        fadeoutType(fadeoutType),
        fps(fps),
        totalFrames(milliseconds / fps)
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
                break;
            case FadeoutType::Desaturate:
                break;
        }

        win.setEventHandler(eventhandler_fadeout);
    }

    void OverlayFadeout::restore(Window& win)
    {
        VisualEffect::restore(win);
    }

    void OverlayFadeout::apply(Window& win)
    {
        install(win);
        win.mainLoop(fps);
        restore(win);
    }

// ========================================================================== //
// fadeout rendering

    bool OverlayFadeout::eventhandler_fadeout(SDL_Event& event, void* userData)
    {
//        FadeoutUserData& userDataStruct = *reinterpret_cast<FadeoutUserData*>(userData);

//        std::cout << (userDataStruct.progress < 1) << std::endl;
//        return userDataStruct.progress < 1;

        return false;
    }

    void OverlayFadeout::render_fadeout(void* userData)
    {
        VisualEffectUserData& userDataStruct = *reinterpret_cast<VisualEffectUserData*>(userData);

//        auto& surface = userDataStruct.windowSurface;
//        std::cout << userDataStruct.progress << std::endl;

        userDataStruct.progress += .1;
    }


}
