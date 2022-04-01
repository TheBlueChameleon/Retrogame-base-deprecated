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
                size_t        frameID;
                double        progress;
                VisualEffect* effectInstanceData;

                UserData(VisualEffect* effectInstanceData, Window* window);
                ~UserData();

                void updateTexture();
            };

        private:
            std::function<bool (SDL_Event& event, void* userData)>  oldEventHandler;
            std::function<void (void* userData)>                    oldIdleHandler;
            void*                                                   oldUserData;

            std::unique_ptr<UserData> userdata;

            void install(Window& win);
            void restore(Window& win);

        protected:
            double        fps;
            size_t        totalFrames;
            double        progressPerFrame;

            // -------------------------------------------------------------- //
            // CTor, DTor

            VisualEffect(const double fps, size_t totalFrames);                                     // making this protected forces use of derived class

            // -------------------------------------------------------------- //
            // VisualEffect interface

            virtual std::function<void (void*)> getRenderer() = 0;
            virtual void prepareInstance(UserData& userData);
            virtual void tidyUpInstance (UserData& userData);

        public:
            virtual void apply(Window& win);

            void renderStoredState();
            virtual void progress();

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

            static VisualEffect::UserData& castToUserData(void* userData);

            template <class T>
            static std::tuple<VisualEffect::UserData&, Window&, T&> unpackUserdataPointer(void* userData)
            {
                auto& userDataStruct = castToUserData(userData);

                auto& win = *userDataStruct.window;
                auto& self = *reinterpret_cast<T*>(userDataStruct.effectInstanceData);

                return std::make_tuple(std::reference_wrapper(userDataStruct),
                                       std::reference_wrapper(win),
                                       std::reference_wrapper(self));
            }
    };
}

#endif // VISUALEFFECT_HPP
