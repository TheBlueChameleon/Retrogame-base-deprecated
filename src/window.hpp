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

// ========================================================================== //
// Class

namespace RetrogameBase {
    class Window {
        private:
            SDL_Window*   hwin         = nullptr;
            SDL_Renderer* win_renderer = nullptr;

        public:
            // ---------------------------------------------------------------------- //
            // CTor, DTor

            Window() = delete;
            Window(const char*        title, int width = 800, int height = 600, Uint32 render_flags = SDL_RENDERER_ACCELERATED);
            Window(const std::string& title, int width = 800, int height = 600, Uint32 render_flags = SDL_RENDERER_ACCELERATED);
            ~Window();

            // ---------------------------------------------------------------------- //
            // getters

            bool isReady() const;

            const char* getTitle() const;

            int getWidth() const;
            int getHeight() const;
            std::pair<int, int> getDimension() const;

            int getPosX() const;
            int getPosY() const;
            std::pair<int, int> getPosition() const;

            Uint32 GetWindowFlags() const;

            SDL_Renderer* getRenderer() const;

            // ---------------------------------------------------------------------- //
            // place, hide and show

            void setTitle(const char* title);
            void setTitle(const std::string& title);

            void setDimension(const int w, const int h);
            void setPosition(const int x, const int y);

            void hide();
            void show();

            void minimize();
            void maximize();
            void restore();

            void update();


    };

}
#endif // WINDOW_HPP
