#include "engine/window.h"

Window::Window()
{
    window = nullptr;
}

Window::~Window()
{
}

bool Window::create(const char* title, int width, int height)
{
    if (!SDL_Init(SDL_INIT_VIDEO))
        return false;

    window = SDL_CreateWindow(
        title,
        width,
        height,
        0
    );

    return window != nullptr;
}

void Window::destroy()
{
    if (window != nullptr)
        SDL_DestroyWindow(window);

    SDL_Quit();
}

SDL_Window* Window::getWindow()
{
    return window;
}