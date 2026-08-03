# 06_Utility_System

This document specifies the software requirements (SRS) for the Utility System module.

---

## 1. Subsystem Specification

* **Identity:** `shape::utility`
* **Purpose:** Decoupled event queues, configurations serialization, and string hashing.
* **Objectives:** Async event delivery, clean configuration updates.
* **Requirements:** Fixed event buffer sizes, compile-time string hashing validations.
* **Responsibilities:** Enqueueing event structs, writing configurations, calculating hashes.
* **Subsystems:** EventBus, ConfigParser, HashingRegistry.
* **Managers:** `UtilityManager`.
* **Components:** None.
* **Interfaces:** `IEventDispatcher`.
* **Events:** None (dispatches all game event types).
* **Resources:** External JSON config files.
* **Algorithms:** Compile-time MurmurHash or FNV-1a updates.
* **Data Structures:** Fixed-size event ring buffers.
* **Execution:** Event bus flushed at Phase 1 starts (reads previous tick events).
* **Dependencies:** `shape::core`
* **Configuration:** Load config properties at start.
* **Serialization:** Saves current active settings as JSON or YAML streams.
* **Debugging:** Output config parses warnings to logs.
* **Testing:** Asserts string hashing contains no collisions for default system tags.
* **Benchmarks:** Timing benchmarks for event queue pushes.
* **Performance Targets:** Amortized $O(1)$ queue updates.
* **Future Extensions:** Dynamic hot-reloaded config parameters watcher.

---

## 2. Verification & Validation (V&V)

* **Verification:** Unit tests assert that the event queue overflows safely (retaining older events) if the push count exceeds static capacities.
* **Validation:** Verification that configuration changes modify resource regeneration and metabolic constants correctly, enabling sandbox balancing.
