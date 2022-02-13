// ========================================================================== //
// Depenencies

// STL
#include <iostream>

// own
#include "globals.hpp"
#include "gfxSystem/window.hpp"

// ========================================================================== //

#include <SDL2/SDL_image.h>

int main()
{
    RetrogameBase::initAll();

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
