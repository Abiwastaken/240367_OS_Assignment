#include "os/process.h"

Process::Process(int pid, int x, int y)
{
    this->pid = pid;

    state = ProcessState::Ready;

    arrivalTime = 0;
    burstTime = 5;
    remainingTime = burstTime;
    priority = 1;

    this->x = x;
    this->y = y;

    width = 40;
    height = 40;

    // Different colour for each process
    red = (pid * 70) % 255;
    green = (pid * 130) % 255;
    blue = (pid * 190) % 255;
}

int Process::getPID() const
{
    return pid;
}

ProcessState Process::getState() const
{
    return state;
}

int Process::getX() const
{
    return x;
}

int Process::getY() const
{
    return y;
}

int Process::getWidth() const
{
    return width;
}

int Process::getHeight() const
{
    return height;
}

int Process::getRed() const
{
    return red;
}

int Process::getGreen() const
{
    return green;
}

int Process::getBlue() const
{
    return blue;
}