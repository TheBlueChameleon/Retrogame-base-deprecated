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

// -------------------------------------------------------------------------- //

    VisualEffect::UserData::UserData(
        VisualEffect* effectInstanceData,
        Window*       window
    ) :
        window            ( window ),
        sdlWindow         ( window->getSdlWindow() ),
        windowRenderer    ( window->getRenderer() ),
        frameID           ( 0 ),
        progress          ( 0. ),
        effectInstanceData( effectInstanceData )
    {
        const auto windowDimensions = window->getDimension();
        const SDL_Rect coordinates = {0, 0, windowDimensions.first, windowDimensions.second};

        const auto format = SDL_GetWindowPixelFormat(sdlWindow);

        // deliberately, make a copy instead of using the existing window surface (https://wiki.libsdl.org/SDL_GetWindowSurface)
        // blur and other mem-poking effects wouldn't work otherwise.
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
        SDL_FreeSurface   (windowSurface);
        SDL_DestroyTexture(windowTexture);
    }

// .......................................................................... //

    void VisualEffect::UserData::updateTexture()
    {
        SDL_DestroyTexture(windowTexture);
        windowTexture = SDL_CreateTextureFromSurface( windowRenderer, windowSurface );
    }

// ========================================================================== //
// VisualEffect interface

    void VisualEffect::install(Window& win)
    {
        oldEventHandler = win.getEventHandler();
        oldIdleHandler  = win.getIdleHandler();
        oldUserData     = win.getUserData();

        userdata = std::unique_ptr<UserData>(new UserData(this, &win));

        win.setUserData    (userdata.get());
        win.setEventHandler(VisualEffect::eventhandler_default);
    }

    void VisualEffect::prepareInstance(UserData& userData) {}
    void VisualEffect::tidyUpInstance (UserData& userData) {}

    void VisualEffect::restore(Window& win)
    {
        win.setEventHandler(oldEventHandler);
        win.setIdleHandler (oldIdleHandler );
        win.setUserData    (oldUserData    );
    }

// .......................................................................... //

    void VisualEffect::apply(Window& win)
    {
        install(win);
        prepareInstance(*userdata);

        win.setIdleHandler(getRenderer());
        win.mainLoop(fps);

        tidyUpInstance(*userdata);
        restore(win);
    }

// -------------------------------------------------------------------------- //

    void VisualEffect::renderStoredState()
    {
        userdata->window->clear(color_black);

        SDL_RenderCopy(userdata->windowRenderer,
                       userdata->windowTexture,
                       nullptr, nullptr);
    }

    void VisualEffect::progress()
    {
        ++userdata->frameID;
        userdata->progress += progressPerFrame;
        pushUserEvent(0, nullptr);
    }

    bool VisualEffect::eventhandler_default(SDL_Event& event, void* userData)
    {
        UserData& userDataStruct = castToUserData(userData);
        return userDataStruct.progress < 1;
    }

// ========================================================================== //
// Getters, Setters

    double VisualEffect::getFps() const
    {
        return fps;
    }

    void VisualEffect::setFps(double newFps)
    {
        fps = newFps;
    }

    size_t VisualEffect::getTotalFrames() const
    {
        return totalFrames;
    }

    void VisualEffect::setTotalFrames(size_t newTotalFrames)
    {
        totalFrames = newTotalFrames;
        progressPerFrame = 1. / totalFrames;
    }

    double VisualEffect::getProgressPerFrame() const
    {
        return progressPerFrame;
    }

    double VisualEffect::getDuration() const
    {
        return totalFrames / fps * 1000;
    }

    void VisualEffect::setDuration(const double milliseconds)
    {
        this->setTotalFrames(fps * milliseconds / 1000.);
    }

// ========================================================================== //
// Helper Functions

    VisualEffect::UserData& castToUserData(void* userData)
    {
        return *reinterpret_cast<VisualEffect::UserData*>(userData);
    }
}
