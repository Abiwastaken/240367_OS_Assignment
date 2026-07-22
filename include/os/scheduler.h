#pragma once

#include <vector>

#include "os/process.h"

class Scheduler
{
public:
    Scheduler();

    void update();

    const std::vector<Process>& getProcesses() const;
    

private:
    std::vector<Process> processes;

    int currentProcess;
    int frameCounter;
    
};
