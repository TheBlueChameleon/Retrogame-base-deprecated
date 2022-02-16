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
#include "../globals.hpp"
#include "texturestore.hpp"

// ========================================================================== //
// Class

namespace RetrogameBase
{
    class TextureStore;

    class Window
    {
        private:
            SDL_Window*   hwin         = nullptr;
            SDL_Renderer* win_renderer = nullptr;

            TextureStore textureStore;

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

            void clear(SDL_Color color = color_black);

            void pset(int  x, int  y,                 SDL_Color color = color_white);
            void line(int x1, int y1, int x2, int y2, SDL_Color color = color_white);
            void  box(int  x, int  y, int  w, int  h, SDL_Color color = color_white);
            void fbox(int  x, int  y, int  w, int  h, SDL_Color color = color_white);

            void print(const char* text,
                       const int x, const int y,
                       int width = -1, int height = -1,
                       SDL_Color color = color_white,
                       TTF_Font* font = nullptr);

            // ---------------------------------------------------------------------- //
            // fadeouts

            enum class FadeoutType
            {
                Stripes,
                Pixelate,
                Desatify
            };

            // ---------------------------------------------------------------------- //
            // storage access

            TextureStore& getTextureStore();

            enum class ResetStoresDepth
            {
                Textures,
                Animations,
                Layers
            };

            void resetStores(ResetStoresDepth depth);
    };
}
#endif // WINDOW_HPP
