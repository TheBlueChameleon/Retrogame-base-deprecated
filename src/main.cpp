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
void showcase_OverlayFadeout(RetrogameBase::Window& win);

// ========================================================================== //
// main

int main()
{
    RetrogameBase::initAll();

    RetrogameBase::Window win("Test Window");

//    showcase_waitTillClose(win);
//    showcase_AnimationLayer(win);
//    showcase_screenshot(win);

    showcase_OverlayFadeout(win);
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
    win.mainLoop(10);
}

void showcase_screenshot(RetrogameBase::Window& win)
{
    win.saveScreenshotPNG("../shot.png", {25, 25, 600, 300});
}

void showcase_OverlayFadeout(RetrogameBase::Window& win)
{
    win.setEventHandler(eventHandler_minimal);
    win.setUserData(&win);
    win.setIdleHandler(
        [] (void* userData)
    {
        auto& win = *reinterpret_cast<RetrogameBase::Window*>(userData);

        win.clear(RetrogameBase::color_blue);

        win.line(0, 0, 800, 600);
        win.line(0, 600, 800, 0);

        win.fbox( 20, 280, 40, 40);
        win.fbox(739, 280, 40, 40);
    }
    );

    win.getIdleHandler()(&win);

    RetrogameBase::SimpleFadeout effect(RetrogameBase::SimpleFadeout::FadeoutType::Stripes, 1000., 30);
    effect.apply(win);

    win.update();
    effect.apply(win);

    win.mainLoop();
}
