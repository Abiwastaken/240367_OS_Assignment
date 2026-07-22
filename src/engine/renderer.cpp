#include "engine/renderer.h"

Renderer::Renderer(Window& window)
{
    renderer = SDL_CreateRenderer(window.getWindow(), nullptr);

    SDL_SetRenderDrawColor(renderer, 25, 25, 35, 255);
}

void Renderer::clear()
{
    SDL_RenderClear(renderer);
}

void Renderer::present()
{
    SDL_RenderPresent(renderer);
}

void Renderer::drawRectangle(int x, int y, int width, int height,
                             int r, int g, int b)
{
    SDL_FRect rect;

    rect.x = x;
    rect.y = y;
    rect.w = width;
    rect.h = height;

    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_RenderFillRect(renderer, &rect);

    // Reset background colour
    SDL_SetRenderDrawColor(renderer, 25, 25, 35, 255);
}