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
                VisualEffect* effectInstanceData;

                UserData(VisualEffect* effectInstanceData, Window* window);
                ~UserData();
            };

        private:
            std::function<bool (SDL_Event& event, void* userData)>  oldEventHandler;
            std::function<void (void* userData)>                    oldIdleHandler;
            void*                                                   oldUserData;

            std::unique_ptr<UserData> userdata;

        protected:
            const double        fps;
            const size_t        totalFrames;
            const double        progressPerFrame;

            VisualEffect(const double fps, size_t totalFrames);                                     // forces use of derived class

            virtual void install(Window& win);
            virtual void restore(Window& win);

        public:
            virtual void apply  (Window& win);

            void renderStoredState();
            virtual void progress();

            static UserData& castToUserData(void* userData);

            static bool eventhandler_default(SDL_Event& event, void* userData);

            double getFps() const;
            size_t getTotalFrames() const;
    };
}

#endif // VISUALEFFECT_HPP
