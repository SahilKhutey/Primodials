# 12_Debugging_System

This document specifies the engine logging system, assertion macros, and memory diagnostic tracking.

---

## 1. Engine Logger Target
The [Logger](file:///C:/Users/ASUS/Documents/Primodilas/Engine/Core/Logger.hpp) maps messages to console and file output:
* **Log Levels:** `INFO`, `WARN`, `ERROR`, `CRITICAL`.
* **Lock-Free Queue:** Messages are processed on a background thread to prevent logging overhead from stalling tick runs.

---

## 2. Invariant Assertions
The engine defines custom assertion macros to enforce correctness checks:
```cpp
#define SHAPE_ASSERT(condition, message) \
    if (!(condition)) { \
        Logger::Critical("Assert Failed: " #condition " - " message); \
        std::abort(); \
    }
```
Used to enforce:
* No negative values in metabolic energy components (System Law 4).
* Valid indices inside the sparse set registry lookups.

---

## 3. Allocator Diagnostic Tracking
To detect memory leaks or OOM issues:
* **Arena Watchdogs:** The `ArenaAllocator` tracks maximum memory usage, logging warnings if consumption exceeds 80% of capacity.
* **Pool Checkers:** The `PoolAllocator` validates free-list indexes on deallocation, catching double-free errors.
