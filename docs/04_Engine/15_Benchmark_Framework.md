# 15_Benchmark_Framework

This document specifies microsecond timing loops, CPU performance counters, and cache profile tests.

---

## 1. Tick Timing Logs
The engine measures execution times of individual systems each tick:
```cpp
u64 start = SDL_GetPerformanceCounter();
PhysicsSystem::Update(world, dt);
u64 end = SDL_GetPerformanceCounter();
f64 elapsedMs = (f64)(end - start) * 1000.0 / (f64)SDL_GetPerformanceFrequency();
```
These durations are logged, allowing developers to identify performance bottlenecks (e.g. narrowphase SAT checks taking > 5ms).

---

## 2. Allocation Overheads Profiling
Benchmarks verify the performance improvements of our custom memory systems:
* Compare execution speeds of `ArenaAllocator` allocation loops against standard heap allocation loops (`new` / `delete`).
* Verify that component memory pools yield constant $O(1)$ timings under high allocation stress.

---

## 3. Cache Miss Profiling
Using profiling tools (e.g. Valgrind, Intel VTune):
* Systems verify L1/L2 cache hit rates during contiguous sparse set updates.
* Iterations over dense arrays must maintain a target L1 data cache miss rate under 2%.
