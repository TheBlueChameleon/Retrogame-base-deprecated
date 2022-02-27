// ========================================================================== //
// Depenencies

// STL
#include <iostream>
#include <stdexcept>
#include <string>
using namespace std::string_literals;

// own
#include "../window.hpp"
#include "visualeffect.hpp"

// ========================================================================== //
// local macro

#define THROWTEXT(msg) ("RUNTIME EXCEPTION IN "s + (__PRETTY_FUNCTION__) + "\n"s + msg)

// ========================================================================== //
// namespace

namespace RetrogameBase
{

// ========================================================================== //
// CTor, DTor

    VisualEffect::VisualEffect(
        const double fps,
        size_t totalFrames
    ) :
        fps(fps),
        totalFrames(totalFrames),
        progressPerFrame(1. / totalFrames)
    {}

    VisualEffect::UserData::UserData(
        VisualEffect* effectInstanceData,
        Window*       window
    ) :
        window            ( window ),
        sdlWindow         ( window->getSdlWindow() ),
        windowRenderer    ( window->getRenderer() ),
        progress          ( 0. ),
        effectInstanceData( effectInstanceData )
    {
        const auto windowDimensions = window->getDimension();
        const SDL_Rect coordinates = {0, 0, windowDimensions.first, windowDimensions.second};

        auto format = SDL_GetWindowPixelFormat(sdlWindow);

        windowSurface = SDL_CreateRGBSurfaceWithFormat(
                            0,                          // flags (unused)
                            windowDimensions.first,     // width
                            windowDimensions.second,    // height
                            SDL_BITSPERPIXEL(format),   // color depth
                            format
                        );

        SDL_RenderReadPixels(
            windowRenderer,
            &coordinates,
            format,
            windowSurface->pixels,
            windowSurface->pitch
        );

        windowTexture = SDL_CreateTextureFromSurface( windowRenderer, windowSurface );
    }

    VisualEffect::UserData::~UserData()
    {
        SDL_FreeSurface(windowSurface);
    }

// ========================================================================== //
// visualEffect interface

    void VisualEffect::install(Window& win)
    {
        oldEventHandler = win.getEventHandler();
        oldIdleHandler  = win.getIdleHandler();
        oldUserData     = win.getUserData();

        userdata = std::unique_ptr<UserData>(new UserData(this, &win));

        win.setUserData    (userdata.get());
        win.setEventHandler(VisualEffect::eventhandler_default);
    }

    void VisualEffect::restore(Window& win)
    {
        win.setEventHandler(oldEventHandler);
        win.setIdleHandler (oldIdleHandler );
        win.setUserData    (oldUserData    );
    }

    void VisualEffect::apply(Window& win)
    {
        install(win);
        win.mainLoop(fps);
        restore(win);
    }

    void VisualEffect::progress()
    {
        userdata->progress += progressPerFrame;
        pushUserEvent(0, nullptr);
    }

    VisualEffect::UserData& VisualEffect::castToUserData(void* userData)
    {
        return *reinterpret_cast<UserData*>(userData);
    }

    bool VisualEffect::eventhandler_default(SDL_Event& event, void* userData)
    {
        UserData& userDataStruct = castToUserData(userData);
        return userDataStruct.progress < 1;
    }

}
