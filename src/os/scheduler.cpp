#include "os/scheduler.h"

Scheduler::Scheduler()
{
    processes.emplace_back(1, 100, 100);
    processes.emplace_back(2, 200, 100);
    processes.emplace_back(3, 300, 100);
    processes.emplace_back(4, 400, 100);
    processes.emplace_back(5, 500, 100);
}

void Scheduler::update()
{
    // Scheduling logic will go here later.
}

const std::vector<Process>& Scheduler::getProcesses() const
{
    return processes;
}