# Threading

This document specifies the parallel jobs scheduler, spatial task graphs, and thread synchronization.

---

## 1. Spatial Task Graphs
The simulation splits updates across regional chunks to parallelize execution safely:
* Independent spatial hash regions are updated in parallel on separate threads.
* **Synchronization:** Chunks containing overlapping boundary buffers are processed sequentially on the main thread, preventing race conditions during entity transitions between cells.

---

## 2. Jobs Thread Pool
* **Worker Threads:** Bootstraps $N$ threads, where $N$ matches the CPU's hardware concurrency limit.
* **Work Stealing:** Idle worker threads steal task segments from busy queues, maintaining a balanced thread workload.

---

## 3. Lock-Free Communication
All thread messaging utilizes lock-free ring buffers:
* **Inputs Queue:** UI inputs are written by the render thread and read by the simulation thread using atomic memory barriers.
* **Diagnostics Logs:** Logging strings are queued atomically to prevent threads from locking on disk operations.
