#ifndef OVERLAYFADEOUT_HPP
#define OVERLAYFADEOUT_HPP

// ========================================================================== //
// Depenencies

// STL
#include <functional>
#include <memory>

// SDL
#include <SDL2/SDL.h>

// own
#include "visualeffect.hpp"

// ========================================================================== //
// Class

namespace RetrogameBase
{
    class OverlayFadeout : public VisualEffect
    {
        public:
            enum class FadeoutType
            {
                Stripes,
                Pixelate,
                Desaturate
            };

//            struct FadeoutUserData
//            {
//                Window*       window;
//                SDL_Window*   hwin;
//                SDL_Renderer* windowRenderer;
//                SDL_Surface*  windowSurface;
//                double        progress;

//                FadeoutUserData(Window* window);
//            };

        private:
            const FadeoutType   fadeoutType;
            const double        fps;
            const size_t        totalFrames;

//            std::unique_ptr<FadeoutUserData> userdata;

        protected:
            virtual void install(Window& win);
            virtual void restore(Window& win);

        public:
            OverlayFadeout() = delete;
            OverlayFadeout(const FadeoutType fadeoutType, size_t milliseconds, double fps);

            virtual void apply(Window& win);

            static bool eventhandler_fadeout(SDL_Event& event, void* userData);
            static void render_fadeout(void* userData);
    };
}

#endif // OVERLAYFADEOUT_HPP
