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
void idleHanlder_Testimages(void* userData);

void showcase_waitTillClose(RetrogameBase::Window& win);
void showcase_AnimationLayer(RetrogameBase::Window& win);
void showcase_screenshot(RetrogameBase::Window& win);
void showcase_SimpleFadeout(RetrogameBase::Window& win);

// ========================================================================== //
// main

int main()
{
    RetrogameBase::initAll();

    RetrogameBase::Window win("Test Window");

//    showcase_waitTillClose(win);
//    showcase_AnimationLayer(win);
//    showcase_screenshot(win);

    showcase_SimpleFadeout(win);
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

void idleHanlder_Testimages(void* userData)
{
    auto& win = *reinterpret_cast<RetrogameBase::Window*>(userData);

    win.clear(RetrogameBase::color_blue);

    for     (auto y=20u; y<580; y+=20)
    {
        for (auto x= 0u; x<800; x+=20)
        {
            const double hue       = (x / 800.0) * 360.0;
            const double lightness =  y / 600.0;

            auto color = RetrogameBase::getColorFromHSL(hue, 1.0, lightness);
            win.fbox(x, y, 20, 20, color);
        }
    }

    win.line(0, 0, 800, 600);
    win.line(0, 600, 800, 0);

    win.fbox( 20, 280, 40, 40);
    win.fbox(739, 280, 40, 40);
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

void showcase_SimpleFadeout(RetrogameBase::Window& win)
{
    win.setEventHandler(eventHandler_minimal);
    win.setUserData(&win);
    win.setIdleHandler(idleHanlder_Testimages);

    win.getIdleHandler()(&win);

    RetrogameBase::SimpleFadeout effect(RetrogameBase::SimpleFadeout::FadeoutType::Pixelate, 1000., 30);
    effect.apply(win);

    win.mainLoop();
}
