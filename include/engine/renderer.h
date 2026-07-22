#pragma once

#include <SDL3/SDL.h>

#include "engine/window.h"

class Renderer
{
public:
    Renderer(Window& window);

    void clear();
    void present();
    void drawRectangle(int x, int y, int width, int height,
                   int r, int g, int b);
private:
    SDL_Renderer* renderer;


};