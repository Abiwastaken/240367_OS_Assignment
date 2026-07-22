#pragma once

#include <vector>

#include "os/process.h"
#include <SDL3/SDL.h>

enum class SchedulingAlgorithm
{
    FCFS,
    RoundRobin
};


class Scheduler
{
public:
    Scheduler();

    void update();

    const std::vector<Process>& getProcesses() const;
    

private:
    std::vector<Process> processes;
    SchedulingAlgorithm algorithm;
    int currentProcess;
    Uint64 lastUpdateTime;
    int quantumCounter;
    void updateFCFS();
    void updateRoundRobin();
};
