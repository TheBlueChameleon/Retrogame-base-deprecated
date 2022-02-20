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

void showcase_waitTillClose(RetrogameBase::Window& win);
void showcase_driveAnimationLayer(RetrogameBase::AnimationLayer& al);

// ========================================================================== //
// main

int main()
{
    RetrogameBase::initAll();

    RetrogameBase::Window win("Test Window");
    RetrogameBase::AnimationLayer& al =win.getAnimationLayerStore().addLayer().second;

    al.loadXML("../unittest-xml/animationlayers/scene.xml");
    auto tiles = al.getElements();

    for (auto& tile : tiles)
    {
        auto& coord = tile.second;
        std::cout << tile.first << ": "
                  << "(" << std::get<0>(coord) << ", " << std::get<1>(coord) << ") at " << std::get<2>(coord) << "°"
                  << std::endl;
    }

    showcase_driveAnimationLayer(al);

//    RetrogameBase::Window win1("x");
//    RetrogameBase::Window win2("y");

//    SDL_Surface* surface = IMG_Load("../gfx/sea01.png");
//    SDL_Texture* tex = SDL_CreateTextureFromSurface(win1.getRenderer(), surface);
//    SDL_FreeSurface(surface);

//    SDL_Rect dest;
//    SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
//    dest.x = (800 - dest.w) / 2;
//    dest.y = (600 - dest.h) / 2;

//    SDL_RenderCopy(win1.getRenderer(), tex, NULL, &dest);
//    SDL_RenderCopy(win2.getRenderer(), tex, NULL, &dest);

//    std::cout << surface << std::endl;
//    std::cout << tex << std::endl;

//    auto fps = 30.;
//    bool close = false;
//    SDL_Event event;

//    while (!close) {
//        while (SDL_PollEvent(&event)) {
//            //std::cout << event.type << std::endl;
//            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE) {
//                close = true;
//            }
//        }

//        win1.update();
//        win2.update();

//        SDL_Delay(1000 / fps);
//    }
}

// ========================================================================== //
// showcase implementation

void showcase_waitTillClose(RetrogameBase::Window& win)
{
    auto fps = 30.;
    bool close = false;
    SDL_Event event;

    // *INDENT-OFF*
    while (!close) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE) {close = true;}
        }

        win.update();

        SDL_Delay(1000 / fps);
    }
    // *INDENT-ON*
}

void showcase_driveAnimationLayer(RetrogameBase::AnimationLayer& al)
{
    auto fps = 30.;
    bool close = false;
    SDL_Event event;

    auto& win = al.getWindow();
    auto& aniStore = al.getAnimationStore();

    // *INDENT-OFF*
    while (!close) {
        al.showCurrentPhaseAndAdvance();

        win.update();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE) {close = true;}
        }
        SDL_Delay(1000 / fps);
    }
    // *INDENT-ON*
}
