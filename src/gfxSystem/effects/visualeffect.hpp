#ifndef VISUALEFFECT_HPP
#define VISUALEFFECT_HPP

// ========================================================================== //
// Depenencies

// STL
#include <functional>
#include <memory>

// SDL
#include <SDL2/SDL.h>

// own

// ========================================================================== //
// Class

namespace RetrogameBase
{
    class Window;

    class VisualEffect
    {
        public:
            struct VisualEffectUserData
            {
                Window*       window;
                SDL_Window*   sdlWindow;
                SDL_Renderer* windowRenderer;
                SDL_Surface*  windowSurface;
                double        progress;
                void*         specificUserdata;

                VisualEffectUserData(Window* window);
                ~VisualEffectUserData();
            };

        private:
            std::function<bool (SDL_Event& event, void* userData)> oldEventHandler;
            std::function<void (void* userData)> oldIdleHandler;
            void* oldUserData;

            std::unique_ptr<VisualEffectUserData> userdata;

        protected:
            virtual void install(Window& win);
            virtual void restore(Window& win);

        public:
            virtual void apply  (Window& win) = 0;
    };
}

#endif // VISUALEFFECT_HPP
