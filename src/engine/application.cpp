#include "engine/application.h"

#include "engine/window.h"
#include "engine/renderer.h"
#include "engine/input.h"

Application::Application()
{
    running = true;
}

Application::~Application()
{
}

void Application::run()
{
    Window window;

    if (!window.create("OS Simulator", 1280, 720))
        return;

    Renderer renderer(window);
    Input input;

    while (running)
    {
        running = input.process();

        renderer.clear();
        renderer.drawRectangle(
    100,
    100,
    50,
    50,
    0,
    255,
    0
);
        

        renderer.present();
    }

    window.destroy();
}