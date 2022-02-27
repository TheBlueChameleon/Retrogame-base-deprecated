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

    VisualEffect::UserData::UserData(Window* window) :
        window          ( window ),
        sdlWindow       ( window->getSdlWindow() ),
        windowRenderer  ( window->getRenderer() ),
        progress        ( 0. ),
        specificUserdata( nullptr )
    {
        const auto widthAndHeight = window->getDimension();
        const SDL_Rect coordinates = {0, 0, widthAndHeight.first, widthAndHeight.second};

        auto format = SDL_GetWindowPixelFormat(sdlWindow);

        windowSurface = SDL_CreateRGBSurfaceWithFormat(
                            0,                                              // flags
                            widthAndHeight.first, widthAndHeight.second,
                            SDL_BITSPERPIXEL(format),                       // color depth
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

        userdata = std::unique_ptr<UserData>(new UserData(&win));

        win.setUserData(userdata.get());
    }

    void VisualEffect::restore(Window& win)
    {
        win.setEventHandler(oldEventHandler);
        win.setIdleHandler (oldIdleHandler );
        win.setUserData    (oldUserData    );
    }

}
