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
            struct UserData
            {
                Window*       window;
                SDL_Window*   sdlWindow;
                SDL_Renderer* windowRenderer;
                SDL_Surface*  windowSurface;
                SDL_Texture*  windowTexture;
                double        progress;
                void*         specificUserdata;

                UserData(Window* window);
                ~UserData();
            };

        private:
            std::function<bool (SDL_Event& event, void* userData)> oldEventHandler;
            std::function<void (void* userData)> oldIdleHandler;
            void* oldUserData;

            std::unique_ptr<UserData> userdata;

        protected:
            virtual void install(Window& win);
            virtual void restore(Window& win);

        public:
            virtual void apply  (Window& win) = 0;
    };
}

#endif // VISUALEFFECT_HPP
