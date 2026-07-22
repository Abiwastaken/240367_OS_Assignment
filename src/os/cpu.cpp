#include "os/cpu.h"

CPU::CPU()
{
    x = 640;
    y = 360;
}

void CPU::update()
{
}

int CPU::getX() const
{
    return x;
}

int CPU::getY() const
{
    return y;
}

void CPU::setPosition(int x, int y)
{
    this->x = x;
    this->y = y;
}