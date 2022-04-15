// ========================================================================== //
// Depenencies

// STL
#include <iostream>
#include <chrono>

// SDL
// #include <SDL2/SDL_image.h>

// own
#include "Retrogamebase.h"

// ========================================================================== //
// showcase defs

bool eventHandler_minimal(SDL_Event& event, void* userData);
void ildeHandler_AnimationLayer(void* userData);
void idleHanlder_Testimages(void* userData);

void install_TestImage(RetrogameBase::Window& win);

void showcase_waitTillClose(RetrogameBase::Window& win);
void showcase_AnimationLayer(RetrogameBase::Window& win);
void showcase_screenshot(RetrogameBase::Window& win);
void showcase_SimpleFadeout(RetrogameBase::Window& win);
void showcase_StripesFadeout(RetrogameBase::Window& win);
void showcase_StripesFadeout(RetrogameBase::Window& win);

// ========================================================================== //
// main

int main()
{
    RetrogameBase::initAll();

    RetrogameBase::Window win("Test Window");

//    showcase_waitTillClose(win);
//    showcase_AnimationLayer(win);
//    showcase_screenshot(win);

//    showcase_SimpleFadeout(win);
//    showcase_StripesFadeout(win);
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

    for     (auto y=20; y<580; y+=20)
    {
        for (auto x= 0; x<800; x+=20)
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
// convenience

void install_TestImage(RetrogameBase::Window& win)
{
    win.setEventHandler(eventHandler_minimal);
    win.setUserData(&win);
    win.setIdleHandler(idleHanlder_Testimages);
}

void timeAndApplyEffect(RetrogameBase::VisualEffect& effect, RetrogameBase::Window& win, const std::string& effectName = "Effect")
{
    auto tic = std::chrono::high_resolution_clock::now();
    effect.apply(win);
    auto toc = std::chrono::high_resolution_clock::now();
    auto dur = toc - tic;

    auto timeActual_ms = dur.count() / 1000000;
    std::cout << effectName << " took " << timeActual_ms << " ms while planned animation time was " << effect.getDuration() << " ms" << std::endl;
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

/*
void showcase_SimpleFadeout(RetrogameBase::Window& win)
{
    install_TestImage(win);

    RetrogameBase::SimpleFadeout effect(RetrogameBase::SimpleFadeout::FadeoutType::Desaturate,
                                        1000., 10);

    win.render();
    timeAndApplyEffect(effect, win, "Desaturate");

    effect.setFadeoutType(RetrogameBase::SimpleFadeout::FadeoutType::Pixelate);
    win.render();
    timeAndApplyEffect(effect, win, "Pixelate");

    effect.setFadeoutType(RetrogameBase::SimpleFadeout::FadeoutType::Blur);
    win.render();
    timeAndApplyEffect(effect, win, "Blur");
}

void showcase_StripesFadeout(RetrogameBase::Window& win)
{
    install_TestImage(win);

    RetrogameBase::StripesFadeout effect(RetrogameBase::StripesFadeout::FadeoutType::Contra,
                                         RetrogameBase::StripesFadeout::Orientation::Vertical,
                                         1000., 30);
    effect.setNStripes(200);
    win.render();
    timeAndApplyEffect(effect, win, "Stripes:Contra, Vertical, 200");

    effect.setNStripes(1);
    effect.setOrientation(RetrogameBase::StripesFadeout::Orientation::Horizontal);
    win.render();
    timeAndApplyEffect(effect, win, "Stripes:Contra, Horizontal, 1");



    effect.setFadeoutType(RetrogameBase::StripesFadeout::FadeoutType::CloseCenter);
    effect.setOrientation(RetrogameBase::StripesFadeout::Orientation::Vertical);
    effect.setNStripes(4);
    win.render();
    timeAndApplyEffect(effect, win, "Stripes:CloseCenter, Vertical, 4");



    effect.setOrientation(RetrogameBase::StripesFadeout::Orientation::Horizontal);
    win.render();
    timeAndApplyEffect(effect, win, "Stripes:CloseCenter, Horizontal, 4");

    effect.setNStripes(1);
    win.render();
    timeAndApplyEffect(effect, win, "Stripes:CloseCenter, Horizontal, 1");



    effect.setFadeoutType(RetrogameBase::StripesFadeout::FadeoutType::Random);
    effect.setNStripes(50);
    win.render();
    timeAndApplyEffect(effect, win, "Stripes:Random, Horizontal, 50");


    effect.setOrientation(RetrogameBase::StripesFadeout::Orientation::Vertical);
    win.render();
    timeAndApplyEffect(effect, win, "Stripes:Random, Vertical, 50");
}
*/
