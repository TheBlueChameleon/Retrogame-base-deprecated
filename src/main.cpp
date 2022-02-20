// ========================================================================== //
// Depenencies

// STL
#include <iostream>

// SDL
// #include <SDL2/SDL_image.h>

// own
#include "Retrogamebase.h"

// ========================================================================== //
// showcase defs

bool eventHandler_minimal(SDL_Event& event, void* userData);
void ildeHandler_AnimationLayer(void* userData);

void showcase_waitTillClose(RetrogameBase::Window& win);
void showcase_AnimationLayer(RetrogameBase::Window& win);
void showcase_screenshot(RetrogameBase::Window& win);

// ========================================================================== //
// main

int main()
{
    RetrogameBase::initAll();

    RetrogameBase::Window win("Test Window");

//    showcase_waitTillClose(win);
//    showcase_AnimationLayer(win);
//    showcase_screenshot(win);
}

// ========================================================================== //
// example handlers

bool eventHandler_minimal(SDL_Event& event, void* userData)
{
    if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE)
    {
        return false;
    }

    return true;
}

void ildeHandler_AnimationLayer(void* userData)
{
    RetrogameBase::Window& win = *reinterpret_cast<RetrogameBase::Window*>(userData);
    win.getAnimationLayerStore().getLayer(0).showCurrentPhaseAndAdvance();
}

// ========================================================================== //
// showcase implementation

void showcase_waitTillClose(RetrogameBase::Window& win)
{
    win.setEventHandler(eventHandler_minimal);
    win.mainLoop();
}

void showcase_AnimationLayer(RetrogameBase::Window& win)
{
    RetrogameBase::AnimationLayer& al = win.getAnimationLayerStore().addLayer().second;

    al.loadXML("../unittest-xml/animationlayers/scene.xml");

    win.setEventHandler(eventHandler_minimal);
    win.setIdleHandler(ildeHandler_AnimationLayer);
    win.setUserData(&win);
    win.mainLoop();
}

void showcase_screenshot(RetrogameBase::Window& win)
{
    win.saveScreenshotPNG("../shot.png", {25, 25, 600, 300});
}

