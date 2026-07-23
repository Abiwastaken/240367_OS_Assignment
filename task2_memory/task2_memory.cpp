
// TASK 2: Memory Management Simulation
// Simulates virtual memory paging:
//   - a program asks for a sequence of pages (numbers)
//   - we only have a small number of "frames" (slots of RAM)
//   - if a page is already loaded  -> HIT
//   - if not                        -> PAGE FAULT (load it in,
//     evicting something if we're full)
//
// Two eviction strategies are compared: FIFO and LRU.
//
// Compile with:  g++ -std=c++20 task2_memory.cpp -o task2
// Run with:      ./task2


#include <iostream>
#include <vector>
#include <list>
#include <algorithm>

struct PagingResult
{
    int pageFaults = 0;
    int pageHits = 0;
};

// FIFO: keep a simple queue of pages currently in memory. The
// page at the FRONT of the queue has been there longest, so
// it's the one we evict when we need room.
PagingResult simulateFIFO(const std::vector<int>& referenceString, int numFrames)
{
    PagingResult result;
    std::list<int> frames; // front = oldest page

    for (int page : referenceString)
    {
        bool hit = std::find(frames.begin(), frames.end(), page) != frames.end();

        if (hit)
        {
            result.pageHits++;
            std::cout << "  request " << page << " -> HIT\n";
        }
        else
        {
            result.pageFaults++;
            std::string evictedMsg;
            if ((int)frames.size() >= numFrames)
            {
                evictedMsg = " (evicted " + std::to_string(frames.front()) + ")";
                frames.pop_front();
            }
            frames.push_back(page);
            std::cout << "  request " << page << " -> FAULT" << evictedMsg << "\n";
        }
    }
    return result;
}

// LRU: every time a page is used, move it to the "most recently
// used" end of the list. When we must evict, remove from the
// "least recently used" end (the front).
PagingResult simulateLRU(const std::vector<int>& referenceString, int numFrames)
{
    PagingResult result;
    std::list<int> frames; // front = least recently used, back = most recently used

    for (int page : referenceString)
    {
        auto it = std::find(frames.begin(), frames.end(), page);
        bool hit = (it != frames.end());

        if (hit)
        {
            result.pageHits++;
            frames.erase(it);
            frames.push_back(page); // mark as most recently used
            std::cout << "  request " << page << " -> HIT\n";
        }
        else
        {
            result.pageFaults++;
            std::string evictedMsg;
            if ((int)frames.size() >= numFrames)
            {
                evictedMsg = " (evicted " + std::to_string(frames.front()) + ")";
                frames.pop_front();
            }
            frames.push_back(page);
            std::cout << "  request " << page << " -> FAULT" << evictedMsg << "\n";
        }
    }
    return result;
}
// main function
int main()
{
    std::vector<int> referenceString = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};
    int numFrames = 3;

    std::cout << "Reference string: 1 2 3 4 1 2 5 1 2 3 4 5   (frames = " << numFrames << ")\n\n";

    std::cout << "--- FIFO ---\n";
    PagingResult fifo = simulateFIFO(referenceString, numFrames);
    double fifoHitRatio = (double)fifo.pageHits / (fifo.pageHits + fifo.pageFaults);
    std::cout << "FIFO faults: " << fifo.pageFaults << ", hits: " << fifo.pageHits
               << ", hit ratio: " << fifoHitRatio << "\n\n";

    std::cout << "--- LRU ---\n";
    PagingResult lru = simulateLRU(referenceString, numFrames);
    double lruHitRatio = (double)lru.pageHits / (lru.pageHits + lru.pageFaults);
    std::cout << "LRU faults: " << lru.pageFaults << ", hits: " << lru.pageHits
               << ", hit ratio: " << lruHitRatio << "\n";

    return 0;
}
