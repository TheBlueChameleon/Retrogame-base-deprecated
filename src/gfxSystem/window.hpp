#ifndef WINDOW_HPP
#define WINDOW_HPP

// ========================================================================== //
// Depenencies

// STL
#include <string>
#include <functional>
#include <utility>

// SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// own
#include "../base/globals.hpp"
#include "texturestore.hpp"
#include "animationstore.hpp"
#include "animationlayer.hpp"
#include "animationlayerstore.hpp"

// ========================================================================== //
// Class

namespace RetrogameBase
{
    class Window
    {
        private:
            SDL_Window*   hwin         = nullptr;
            SDL_Renderer* win_renderer = nullptr;

            TextureStore        textureStore;
            AnimationStore      animationStore;
            AnimationLayerStore animationLayerStore;

            std::function<bool (SDL_Event& event, void* userData)> eventHandler;
            std::function<void (void* userData)> idleHandler;
            void* userData;

        public:
            // ---------------------------------------------------------------------- //
            // CTor, DTor

            Window() = delete;
            Window(const char* title, int width = 800, int height = 600, Uint32 render_flags = SDL_RENDERER_ACCELERATED);
            ~Window();

            // ---------------------------------------------------------------------- //
            // getters

            const char* getTitle() const;

            int getWidth() const;
            int getHeight() const;
            std::pair<int, int> getDimension() const;

            int getPosX() const;
            int getPosY() const;
            std::pair<int, int> getPosition() const;

            Uint32 GetWindowFlags() const;

            SDL_Window*   getSdlWindow() const;
            SDL_Renderer* getRenderer() const;

            // ---------------------------------------------------------------------- //
            // place, hide and show

            void setTitle(const char* title) const;
            void setTitle(const std::string& title) const;

            void setDimension(const int w, const int h) const;
            void setPosition(const int x, const int y) const;

            void hide() const;
            void show() const;

            void minimize() const;
            void maximize() const;
            void restore() const;

            void update() const;

            // ---------------------------------------------------------------------- //
            // drawing primitives

            void clear(SDL_Color color = color_black) const;

            void pset(int  x, int  y,                 SDL_Color color = color_white) const;
            void line(int x1, int y1, int x2, int y2, SDL_Color color = color_white) const;
            void  box(int  x, int  y, int  w, int  h, SDL_Color color = color_white) const;
            void fbox(int  x, int  y, int  w, int  h, SDL_Color color = color_white) const;

            void print(const char* text,
                       const int x, const int y,
                       int width = -1, int height = -1,
                       SDL_Color color = color_white,
                       TTF_Font* font = nullptr) const;

            void saveScreenshotPNG(const std::string& filename) const;
            void saveScreenshotPNG(const std::string& filename, const SDL_Rect& coordinates) const;

            // ---------------------------------------------------------------------- //
            // storage access

            TextureStore&        getTextureStore       ();
            AnimationStore&      getAnimationStore     ();
            AnimationLayerStore& getAnimationLayerStore();

            enum class ResetStoresDepth
            {
                Textures,
                Animations,
                Layers
            };
            void resetStores(ResetStoresDepth depth);

            // ---------------------------------------------------------------------- //
            // event handling

            void setEventHandler(const std::function<bool (SDL_Event& event, void* userData)>& eventHandler);
            const std::function<bool (SDL_Event& event, void* userData)> getEventHandler() const;

            void setIdleHandler(const std::function<void (void* userData)>& idleHandler);
            const std::function<void (void* userData)> getIdleHandler() const;

            bool distributeEvents();
            void mainLoop        (double fps = 30);

            void* getUserData() const;
            void setUserData(void* const newUserData);
    };
}
#endif // WINDOW_HPP
