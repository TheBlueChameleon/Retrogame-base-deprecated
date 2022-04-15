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
        private:
            std::function<bool (SDL_Event& event, void* userData)>  oldEventHandler;
            std::function<void (void* userData)>                    oldIdleHandler;
            void*                                                   oldUserData;

            void install(Window& win);
            void restore(Window& win);

        protected:
            double        fps;
            size_t        totalFrames;
            double        progressPerFrame;

            size_t        frameID;
            double        progress;

            SDL_Surface*  windowSurface;
            SDL_Texture*  windowTexture;

            Window*       window;
            SDL_Window*   sdlWindow;
            SDL_Renderer* windowRenderer;

            void updateStoredTexture();
            void renderStoredState();

            // -------------------------------------------------------------- //
            // CTor, DTor

            VisualEffect(const double fps, size_t totalFrames);                                     // making this protected forces use of derived class

            // -------------------------------------------------------------- //
            // VisualEffect interface

            virtual std::function<void (void*)> getRenderer() = 0;
            virtual void prepareInstance();
            virtual void tidyUpInstance ();

        public:
            virtual void apply(Window& win);

            virtual void advanceFrame();

            static bool eventhandler_default(SDL_Event& event, void* userData);

            // -------------------------------------------------------------- //
            // Getters, Setters

            double getFps() const;
            void   setFps(double newFps);

            size_t getTotalFrames() const;
            void   setTotalFrames(size_t newTotalFrames);

            double getDuration() const;
            void   setDuration(const double milliseconds);

            double getProgressPerFrame() const;

            // -------------------------------------------------------------- //
            // Helper Functions

            template <class T>
            static T& castToVisualEffect(void* userData)
            {
                return *reinterpret_cast<T*>(userData);
            }
    };
}

#endif // VISUALEFFECT_HPP
