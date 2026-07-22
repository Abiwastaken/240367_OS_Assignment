#include "os/scheduler.h"

Scheduler::Scheduler()
{
    processes.emplace_back(1,100,100);
    processes.emplace_back(2,200,100);
    processes.emplace_back(3,300,100);
    processes.emplace_back(4,400,100);
    processes.emplace_back(5,500,100);

    algorithm = SchedulingAlgorithm::RoundRobin;

    currentProcess = 0;
    lastUpdateTime = SDL_GetTicks();
    quantumCounter = 0;

    processes[0].setState(ProcessState::Running);
}

void Scheduler::update()
{
    switch(algorithm)
    {
        case SchedulingAlgorithm::FCFS:
            updateFCFS();
            break;

        case SchedulingAlgorithm::RoundRobin:
            updateRoundRobin();
            break;
    }
}

void Scheduler::updateFCFS()
{
    Uint64 currentTime = SDL_GetTicks();

if (currentTime - lastUpdateTime < 1000)
    return;

lastUpdateTime = currentTime;

    Process& process = processes[currentProcess];

    process.setRemainingTime(
        process.getRemainingTime()-1
    );

    if(process.getRemainingTime() <= 0)
    {
        process.setState(ProcessState::Finished);

        currentProcess++;

        if(currentProcess < processes.size())
            processes[currentProcess].setState(ProcessState::Running);
    }
}

void Scheduler::updateRoundRobin()
{
    if(currentProcess >= processes.size())
        return;

    Uint64 currentTime = SDL_GetTicks();

if (currentTime - lastUpdateTime < 1000)
    return;

lastUpdateTime = currentTime;

    Process& process = processes[currentProcess];

    process.setRemainingTime(
        process.getRemainingTime()-1
    );

    quantumCounter++;

    if(process.getRemainingTime() <= 0)
    {
        process.setState(ProcessState::Finished);

        quantumCounter = 0;

        currentProcess++;

        while(currentProcess < processes.size() &&
              processes[currentProcess].getState()==ProcessState::Finished)
        {
            currentProcess++;
        }

        if(currentProcess < processes.size())
            processes[currentProcess].setState(ProcessState::Running);

        return;
    }

    if(quantumCounter >= 2)
    {
        quantumCounter = 0;

        process.setState(ProcessState::Ready);

        int next = (currentProcess+1)%processes.size();

        while(processes[next].getState()==ProcessState::Finished)
        {
            next=(next+1)%processes.size();

            if(next==currentProcess)
                return;
        }

        currentProcess=next;

        processes[currentProcess].setState(ProcessState::Running);
    }
}

const std::vector<Process>& Scheduler::getProcesses() const
{
    return processes;
}