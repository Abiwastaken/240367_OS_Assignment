cmake_minimum_required(VERSION 3.16)
project(OSAssignment)
 
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
 
find_package(Threads REQUIRED)
 
# --- Task 1: threading, mutex, semaphore, round-robin, race condition, deadlock ---
add_executable(task1 task1_threads/task1_threads.cpp)
target_link_libraries(task1 PRIVATE Threads::Threads)
 
# --- Task 2: paging simulator (FIFO + LRU) ---
add_executable(task2 task2_memory/task2_memory.cpp)
 
# --- Task 3: secure file system (auth, permissions, encryption, audit log) ---
add_executable(task3 task3_filesystem/task3_filesystem.cpp)
 
# --- Task 4: real client-server over TCP sockets ---
add_executable(server task4_network/server.cpp)
target_link_libraries(server PRIVATE Threads::Threads)
 
add_executable(client task4_network/client.cpp)
 

