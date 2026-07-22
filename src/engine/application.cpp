#include "engine/application.h"
#include "os/scheduler.h"
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
    
    Scheduler scheduler;

    while (running)
    {
        running = input.process();

        renderer.clear();
       scheduler.update();

for (const Process& process : scheduler.getProcesses())
{
    renderer.drawRectangle(
        process.getX(),
        process.getY(),
        process.getWidth(),
        process.getHeight(),
        process.getRed(),
        process.getGreen(),
        process.getBlue()
    );
}
        

        renderer.present();
    }

    window.destroy();
}