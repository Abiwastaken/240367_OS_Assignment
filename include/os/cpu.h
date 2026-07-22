#pragma once

class CPU
{
public:
    CPU();

    void update();

    int getX() const;
    int getY() const;

    void setPosition(int x, int y);

private:
    int x;
    int y;
};