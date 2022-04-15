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
            static constexpr int USE_FULL_WINDOW = -1;

        private:
            std::function<bool (SDL_Event& event, void* userData)>  oldEventHandler;
            std::function<void (void* userData)>                    oldIdleHandler;
            void*                                                   oldUserData;

            bool resetW, resetH;

            void install(Window& win);
            void restore(Window& win);

        protected:
            double        fps;
            size_t        totalFrames;
            double        progressPerFrame;
            size_t        frameID;
            double        progress;

            int    x, y, w, h;

            Window*       window;
            SDL_Window*   sdlWindow;
            SDL_Renderer* windowRenderer;
            SDL_Surface*  windowSurface;
            SDL_Texture*  windowTexture;

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

            int  getX() const;
            void setX(int newX);

            int  getY() const;
            void setY(int newY);

            std::pair<int, int> getEffectCoordinates() const;
            void setEffectCoordinates(std::pair<int, int> coords);

            size_t getW() const;
            void   setW(size_t newW);

            size_t getH() const;
            void   setH(size_t newH);

            std::pair<int, int> getEffectDimension() const;
            void setEffectDimension(std::pair<int, int> dimension);

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
