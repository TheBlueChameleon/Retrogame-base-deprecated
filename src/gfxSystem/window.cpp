// ========================================================================== //
// Depenencies

// STL
#include <iostream>
#include <exception>

// local
#include "window.hpp"

// ========================================================================== //
// namespace

namespace RetrogameBase {

// ========================================================================== //
// CTor, DTor

    Window::Window(const char* title, int width, int height, Uint32 render_flags) {
        hwin = SDL_CreateWindow(title,
                                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,     // x, y
                                width, height,
                                SDL_WINDOW_SHOWN);

        win_renderer = SDL_CreateRenderer(hwin, -1, render_flags);
        // -1 is "index of driver"; -1 stands for "first supporting driver"

        if (!(hwin && win_renderer)) {
            throw std::runtime_error("Window could not be initialized.");
        }
    }

    Window::~Window() {
        SDL_DestroyRenderer(win_renderer);
        SDL_DestroyWindow(hwin);

        hwin         = nullptr;
        win_renderer = nullptr;
    }

// ========================================================================== //
// getters

    const char* Window::getTitle() const {
        return SDL_GetWindowTitle(hwin);
    }

    int Window::getWidth() const {
        return SDL_GetWindowSurface(hwin)->w;
    }

    int Window::getHeight() const {
        return SDL_GetWindowSurface(hwin)->h;
    }

    std::pair<int, int> Window::getDimension() const {
        int w, h;
        SDL_GetWindowSize(hwin, &w, &h);

        return std::make_pair(w, h);
    }

    int Window::getPosX() const {
        return getPosition().first;
    }

    int Window::getPosY() const {
        return getPosition().second;
    }

    std::pair<int, int> Window::getPosition() const {
        int x, y;
        SDL_GetWindowPosition(hwin, &x, &y);
        return std::make_pair(x, y);
    }

    Uint32 Window::GetWindowFlags() const {
        return SDL_GetWindowFlags(hwin);
    }

    SDL_Renderer* Window::getRenderer() const {
        return win_renderer;
    }

    // ========================================================================== //
    // place, hide and show

    void Window::setTitle(const char* title) {
        SDL_SetWindowTitle(hwin, title);
    }

    void Window::setTitle(const std::string& title) {
        setTitle(title.c_str());
    }

    void Window::setDimension(const int w, const int h) {
        SDL_SetWindowSize(hwin, w, h);
    }

    void Window::setPosition(const int x, const int y) {
        SDL_SetWindowPosition(hwin, x, y);
    }

    void Window::hide() {
        SDL_HideWindow(hwin);
    }

    void Window::show() {
        SDL_ShowWindow(hwin);
    }

    void Window::minimize() {
        SDL_MinimizeWindow(hwin);
    }

    void Window::maximize() {
        SDL_MaximizeWindow(hwin);
    }

    void Window::restore() {
        SDL_RestoreWindow(hwin);
    }

    void Window::update() {
        SDL_RenderPresent(win_renderer);
    }


// ========================================================================== //
// namespace

}
