#pragma once

enum class ProcessState
{
    Ready,
    Running,
    Waiting,
    Finished
};

class Process
{
public:
        Process(int pid, int x, int y);

    int getPID() const;
    ProcessState getState() const;

    int getX() const;
    int getY() const;

    int getWidth() const;
    int getHeight() const;

    int getRed() const;
    int getGreen() const;
    int getBlue() const;
    
    
 

    int getArrivalTime() const;
    int getBurstTime() const;
    int getRemainingTime() const;
    void setBurstTime(int burst);
    void setRemainingTime(int time);
    void setState(ProcessState newState);

private:
    int pid;

    ProcessState state;

    int arrivalTime;
    int burstTime;
    int remainingTime;
    int priority;

    int x;
    int y;

    int width;
    int height;

    int red;
    int green;
    int blue;
};