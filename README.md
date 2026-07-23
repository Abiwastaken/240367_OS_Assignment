# ST5004CEM Coursework — Simple Version

Four small, independent programs. Each one is a single file that does
exactly what its task in the brief asks — nothing extra, no game, no
graphics. Read top to bottom, it's all one linear story per file.

## How to build

```
mkdir build && cd build
cmake ..
cmake --build .
```

This gives you 5 programs: `task1`, `task2`, `task3`, `server`, `client`.

(If you'd rather not use CMake, plain g++ works too — see "Compile with"
at the top of each .cpp file.)

## How to run each one

```
./task1     # Task 1: threading, mutex, semaphore, round-robin, race condition, deadlock
./task2     # Task 2: paging simulator (FIFO + LRU)
./task3     # Task 3: secure file system (auth, permissions, encryption, audit log)
```

Task 4 needs two terminals:
```
./server           # terminal 1 -- leave this running
./client           # terminal 2 -- run this
```

(Task 4 uses POSIX sockets, so it needs Linux or macOS. On Windows use WSL.)

## Proving "multiple concurrent client connections" (Task 4)

This is the part that's easy to get wrong by accident: the server code
handles concurrent clients (every accepted connection gets its own
thread, so `accept()` is immediately free to accept the next one) — but
if you only ever test with ONE client at a time, you'll never actually
SEE that in action, because a single client connects and disconnects
in well under a second.

To make it visible, `client.cpp` pauses for 4 seconds right after
connecting, before it does anything else. That means if you start it in
2 or 3 separate terminals within a few seconds of each other, all of
them are genuinely connected to the server AT THE SAME TIME during that
pause.

**How to test it yourself:**
1. Start `./server` in terminal 1.
2. Start `./client` in terminal 2. Quickly (within ~3 seconds) also start
   `./client` in terminal 3, and again in terminal 4.
3. Watch each client's `LIST` response — it reports how many clients are
   connected to the server *at that exact moment*. If you start all three
   close together you'll see numbers like `CLIENTS 3`, `CLIENTS 2`,
   `CLIENTS 1` as they each finish in turn — proof that all three were
   genuinely connected simultaneously, not queued one after another.

That's your screenshot/evidence for this requirement — screenshot all
3 client terminals side by side, plus the server terminal showing
multiple "client connected" lines before any "client disconnected" line.

## What's in each file, in one sentence

- **`task1_threads.cpp`** — 4 threads share a printer (`std::mutex`) and 2
  parking spots (`std::counting_semaphore`); a round-robin scheduler runs
  3 fake processes; the same counter is incremented unsafely (wrong
  result — the race condition) vs. safely (correct, mutex-protected);
  two threads grab two shared locks together with `std::lock()` so they
  can never deadlock.
- **`task2_memory.cpp`** — runs the same page-request sequence through
  FIFO and LRU eviction, printing each hit/fault and the final fault
  count and hit ratio for both.
- **`task3_filesystem.cpp`** — `User` stores a password *hash*, never the
  real password; `File` has owner-vs-others read/write permissions and
  XOR encryption; `FileSystem` checks permissions before every action
  and logs every attempt (allowed or denied) with a timestamp.
- **`server.cpp` / `client.cpp`** — a real TCP server that spawns one
  thread per connected client, with a simple text protocol
  (`LOGIN`/`MSG`/`LIST`/`QUIT`); the client logs in and talks to it, and
  pauses briefly so you can test multiple clients at once (see above).

