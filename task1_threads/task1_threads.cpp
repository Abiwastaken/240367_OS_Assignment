
// TASK 1: Process Management and Threading
// 
// This ONE file demonstrates every requirement:
//   1. Multiple threads running at once           -> section A
//   2. Synchronization: a mutex AND a semaphore    -> section A
//   3. A round-robin scheduler simulation          -> section B
//   4. A race condition, and how to prevent it     -> section C
//   5. A deadlock, and how to prevent it           -> section D
//
// Compile with:  g++ -std=c++20 task1_threads.cpp -pthread -o task1
// Run with:      ./task1


#include <iostream>
#include <thread>
#include <mutex>
#include <semaphore>
#include <vector>
#include <queue>
#include <string>


// SECTION A: 3+ threads sharing one resource, protected by a
// mutex (only 1 thread inside at a time) and a semaphore (only
// N threads inside at a time).


std::mutex printerMutex;                 // only ONE thread may use the printer
std::counting_semaphore<10> parkingSpots(2); // only TWO threads may "park" at once

void useSharedPrinter(int threadId)
{
    std::lock_guard<std::mutex> lock(printerMutex); // locks automatically, unlocks when it goes out of scope
    std::cout << "[Thread " << threadId << "] printing...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    std::cout << "[Thread " << threadId << "] done printing.\n";
}

void useParkingSpot(int threadId)
{
    std::cout << "[Thread " << threadId << "] waiting for a parking spot...\n";
    parkingSpots.acquire();               // blocks here if both spots are taken
    std::cout << "[Thread " << threadId << "] got a spot!\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    parkingSpots.release();                // frees the spot for someone else
    std::cout << "[Thread " << threadId << "] left the spot.\n";
}

void runThreadingDemo()
{
    std::cout << "\n SECTION A: threads + mutex + semaphore \\n";

    std::vector<std::thread> threads;
    for (int i = 1; i <= 4; i++)          
    {
        threads.emplace_back([i]() {
            useSharedPrinter(i);
            useParkingSpot(i);
        });
    }
    for (auto& t : threads) t.join();
}


// SECTION B: Round-robin scheduler simulation.
// Each process gets a fixed "quantum" (time slice) of CPU time.
// If it isn't finished, it goes to the back of the queue.


struct SimpleProcess
{
    int id;
    int remainingTime;
};

void runRoundRobinDemo()
{
    std::cout << "\n===== SECTION B: round-robin scheduler =====\n";

    const int QUANTUM = 2; // each process gets 2 units of CPU time per turn

    std::queue<SimpleProcess> readyQueue;
    readyQueue.push({1, 5});
    readyQueue.push({2, 3});
    readyQueue.push({3, 7});

    while (!readyQueue.empty())
    {
        SimpleProcess p = readyQueue.front();
        readyQueue.pop();

        int timeUsed = std::min(QUANTUM, p.remainingTime);
        p.remainingTime -= timeUsed;

        std::cout << "Process " << p.id << " ran for " << timeUsed
                   << " units (remaining: " << p.remainingTime << ")\n";

        if (p.remainingTime > 0)
            readyQueue.push(p); // not done yet -> back of the queue
        else
            std::cout << "Process " << p.id << " finished.\n";
    }
}


// SECTION C: Race condition -- and the fix.
// Several threads increment the SAME variable. Without a lock,
// updates can be lost (two threads read the same old value and
// both write back old+1). With a lock, it's always correct.


int unsafeCounter = 0;
int safeCounter = 0;
std::mutex counterMutex;

void incrementUnsafe(int times)
{
    for (int i = 0; i < times; i++)
    {
        int temp = unsafeCounter;   // read
        std::this_thread::yield();  // let another thread jump in here -- this does not cause a race condition
        unsafeCounter = temp + 1;   // write (may overwrite someone else's update might cause race condition)
    }
}

void incrementSafe(int times)
{
    for (int i = 0; i < times; i++)
    {
        std::lock_guard<std::mutex> lock(counterMutex); // protected
        safeCounter++;
    }
}

void runRaceConditionDemo()
{
    std::cout << "\n===== SECTION C: race condition demo =====\n";

    const int THREADS = 4;
    const int INCREMENTS = 5000;
    int expected = THREADS * INCREMENTS;

    std::vector<std::thread> unsafeThreads;
    for (int i = 0; i < THREADS; i++) unsafeThreads.emplace_back(incrementUnsafe, INCREMENTS);
    for (auto& t : unsafeThreads) t.join();

    std::vector<std::thread> safeThreads;
    for (int i = 0; i < THREADS; i++) safeThreads.emplace_back(incrementSafe, INCREMENTS);
    for (auto& t : safeThreads) t.join();

    std::cout << "Expected value:      " << expected << "\n";
    std::cout << "Unsafe counter got:  " << unsafeCounter
               << (unsafeCounter != expected ? "   <-- WRONG! lost updates (the race condition)" : "") << "\n";
    std::cout << "Safe counter got:    " << safeCounter
               << (safeCounter == expected ? "   <-- correct, the mutex fixed it" : "") << "\n";
}


// SECTION D: Deadlock -- and the fix.
// Two threads each need two locks. If they grab them in a
// DIFFERENT order, they can deadlock (each waits forever for
// the other's lock). std::lock() grabs both locks together and
// is guaranteed to never deadlock, no matter the order used.


std::mutex lockA, lockB;

void safePairOfLocks(int threadId)
{
    std::lock(lockA, lockB); // acquires BOTH locks together, deadlock-free by design
    std::lock_guard<std::mutex> guardA(lockA, std::adopt_lock);
    std::lock_guard<std::mutex> guardB(lockB, std::adopt_lock);

    std::cout << "[Thread " << threadId << "] safely got both locks\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void runDeadlockDemo()
{
    std::cout << "\n===== SECTION D: deadlock prevention demo =====\n";

    std::thread t1(safePairOfLocks, 1);
    std::thread t2(safePairOfLocks, 2);
    t1.join();
    t2.join();

    std::cout << "Both threads finished -- no deadlock happened.\n";
}

// main fucntion
int main()
{
    runThreadingDemo();
    runRoundRobinDemo();
    runRaceConditionDemo();
    runDeadlockDemo();
    return 0;
}
