#include "os/scheduler.h"

Scheduler::Scheduler()
{
    processes.emplace_back(1, 100, 100);
    processes.emplace_back(2, 200, 100);
    processes.emplace_back(3, 300, 100);
    processes.emplace_back(4, 400, 100);
    processes.emplace_back(5, 500, 100);

    currentProcess = 0;
    frameCounter = 0;

    processes[currentProcess].setState(ProcessState::Running);
}
void Scheduler::update()
{
    if (currentProcess >= processes.size())
        return;

    frameCounter++;

    if (frameCounter < 120)
        return;

    frameCounter = 0;

    Process& process = processes[currentProcess];

    process.setRemainingTime(
        process.getRemainingTime() - 1
    );

    if (process.getRemainingTime() <= 0)
    {
        process.setState(ProcessState::Finished);

        currentProcess++;

        if (currentProcess < processes.size())
        {
            processes[currentProcess].setState(ProcessState::Running);
        }
    }
}
const std::vector<Process>& Scheduler::getProcesses() const
{
    return processes;
}