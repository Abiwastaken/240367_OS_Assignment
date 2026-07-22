#include "os/process.h"

Process::Process(int pid, int x, int y)
{
    this->pid = pid;

    state = ProcessState::Ready;

    arrivalTime = 0;
    burstTime = pid*3;
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
    switch(state)
    {
        case ProcessState::Ready: return 0;
        case ProcessState::Running: return 0;
        case ProcessState::Finished: return 120;
        default: return 255;
    }
}

int Process::getGreen() const
{
    switch(state)
    {
        case ProcessState::Ready: return 120;
        case ProcessState::Running: return 255;
        case ProcessState::Finished: return 120;
        default: return 255;
    }
}

int Process::getBlue() const
{
    switch(state)
    {
        case ProcessState::Ready: return 255;
        case ProcessState::Running: return 0;
        case ProcessState::Finished: return 120;
        default: return 255;
    }
}



void Process::setState(ProcessState newState)
{
    state = newState;
}

int Process::getRemainingTime() const
{
    return remainingTime;
}

void Process::setRemainingTime(int time)
{
    remainingTime = time;
}

int Process::getArrivalTime() const
{
    return arrivalTime;
}

int Process::getBurstTime() const
{
    return burstTime;
}

void Process::setBurstTime(int burst)
{
    burstTime = burst;
    remainingTime = burst;
}