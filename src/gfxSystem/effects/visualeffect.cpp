// ========================================================================== //
// Depenencies

// STL
#include <iostream>
#include <stdexcept>
#include <string>
using namespace std::string_literals;

// own
#include "../../base/globals.hpp"
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
        progressPerFrame(1. / totalFrames),
        frameID(0), progress(0),

        effectX(0), effectY(0),
        effectWidth(USE_FULL_WINDOW),
        effectHeight(USE_FULL_WINDOW),

        window(nullptr),
        sdlWindow(nullptr),
        windowRenderer(nullptr),
        windowSurface(nullptr),
        windowTexture(nullptr)
    {}

// ========================================================================== //
// VisualEffect interface

    void VisualEffect::apply(Window& win)
    {
        install(win);
        prepareInstance();

        win.setIdleHandler( getRenderer() );
        win.mainLoop(fps);

        tidyUpInstance();
        restore(win);
    }

// .......................................................................... //

    void VisualEffect::install(Window& win)
    {
        window            = &win;
        sdlWindow         = win.getSdlWindow();
        windowRenderer    = win.getRenderer();
        frameID           = 0 ;
        progress          = 0.;

        oldEventHandler   = win.getEventHandler();
        oldIdleHandler    = win.getIdleHandler();
        oldUserData       = win.getUserData();

        SDL_RenderGetClipRect(windowRenderer, &oldClippingRegion);

        // *INDENT-OFF*
        if (effectWidth  == USE_FULL_WINDOW) {effectWidth  = win.getWidth ();}
        if (effectHeight == USE_FULL_WINDOW) {effectHeight = win.getHeight();}
        // *INDENT-ON*

        const SDL_Rect effectRegion = {effectX, effectY, effectWidth, effectHeight};
        const auto format = SDL_GetWindowPixelFormat(sdlWindow);

        // deliberately, make a copy instead of using the existing window surface (https://wiki.libsdl.org/SDL_GetWindowSurface)
        // blur and other mem-poking effects wouldn't work otherwise.
        windowSurface = SDL_CreateRGBSurfaceWithFormat(
                            0,                          // flags (unused)
                            effectWidth, effectHeight,  // dimension in pixels
                            SDL_BITSPERPIXEL(format),   // color depth
                            format
                        );
        SDL_RenderReadPixels(
            windowRenderer,
            &effectRegion,
            format,
            windowSurface->pixels,
            windowSurface->pitch
        );

        windowTexture = SDL_CreateTextureFromSurface( windowRenderer, windowSurface );

        win.setUserData    (this);
        win.setEventHandler(VisualEffect::eventhandler_default);

        SDL_RenderSetClipRect(windowRenderer, &effectRegion);
    }

    void VisualEffect::prepareInstance() {}
    void VisualEffect::tidyUpInstance () {}

    void VisualEffect::restore(Window& win)
    {
        // *INDENT-OFF*
        constexpr SDL_Rect nullRect = {0,0,0,0};
        if (oldClippingRegion == nullRect) {SDL_RenderSetClipRect(windowRenderer, nullptr);}
        else                               {SDL_RenderSetClipRect(windowRenderer, &oldClippingRegion);}
        // *INDENT-ON*

        win.setEventHandler(oldEventHandler);
        win.setIdleHandler (oldIdleHandler );
        win.setUserData    (oldUserData    );


        SDL_FreeSurface    (windowSurface);
        SDL_DestroyTexture (windowTexture);

        windowSurface = nullptr;
    }

// -------------------------------------------------------------------------- //

    void VisualEffect::updateStoredTexture()
    {
        SDL_DestroyTexture(windowTexture);
        windowTexture = SDL_CreateTextureFromSurface( windowRenderer, windowSurface );
    }

    void VisualEffect::renderStoredState()
    {
        const SDL_Rect coords = {effectX, effectY, effectWidth, effectHeight};
        SDL_RenderCopy(windowRenderer,
                       windowTexture,
                       nullptr,     // source rect
                       &coords      // dest rect
                      );
    }

// .......................................................................... //

    void VisualEffect::advanceFrame()
    {
        ++frameID;
        progress += progressPerFrame;
        pushUserEvent(0, nullptr);
    }

    bool VisualEffect::eventhandler_default(SDL_Event& event, void* userData)
    {
        auto self = reinterpret_cast<VisualEffect*>(userData);
        return self->frameID < self->totalFrames;
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

    int VisualEffect::getEffectX() const
    {
        return effectX;
    }

    void VisualEffect::setEffectX(int newX)
    {
        effectX = newX;
    }

    int VisualEffect::getEffectY() const
    {
        return effectY;
    }

    void VisualEffect::setEffectY(int newY)
    {
        effectY = newY;
    }

    std::pair<int, int> VisualEffect::getEffectCoordinates() const
    {
        return std::make_pair(effectX, effectY);
    }

    void VisualEffect::setEffectCoordinates(std::pair<int, int> coords)
    {
        std::tie(effectX, effectY) = coords;
    }

    size_t VisualEffect::getEffectWidth() const
    {
        return effectWidth;
    }

    void VisualEffect::setEffectWidth(size_t newW)
    {
        effectWidth = newW;
    }

    size_t VisualEffect::getEffectHeight() const
    {
        return effectHeight;
    }

    void VisualEffect::setEffectHeight(size_t newH)
    {
        effectHeight = newH;
    }

    std::pair<int, int> VisualEffect::getEffectDimension() const
    {
        return std::make_pair(effectWidth, effectHeight);
    }

    void VisualEffect::setEffectDimension(std::pair<int, int> dimension)
    {
        std::tie(effectWidth, effectHeight) = dimension;
    }
}
