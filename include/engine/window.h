#pragma once

#include <SDL3/SDL.h>

class Window
{
public:
    Window();
    ~Window();

    bool create(const char* title, int width, int height);
    void destroy();

    SDL_Window* getWindow();

private:
    SDL_Window* window;
};