# 05_Container_System

This document specifies the software requirements (SRS) for the Container System module.

---

## 1. Subsystem Specification

* **Identity:** `shape::container`
* **Purpose:** Custom dense-sparse array component stores and deferred mutation queues.
* **Objectives:** contiguous memory allocation, fast $O(1)$ random lookups, cache-friendly iterations.
* **Requirements:** Generational index checking, structural mutations deferrals.
* **Responsibilities:** Mapping Entity handles to components offsets, swap-and-pop deletions.
* **Subsystems:** SparseRegistry, DenseArrayStore, CommandBufferQueue.
* **Managers:** None (managed by the `World` instance).
* **Components:** None (manages component storages).
* **Interfaces:** `IComponentStore`.
* **Events:** None.
* **Resources:** Pre-allocated pool memory blocks.
* **Algorithms:** Swap-and-pop array updates, generational mask validations.
* **Data Structures:** Sparse set indices structures.
* **Execution:** Commands execution deferred to Phase 3 flushes.
* **Dependencies:** `shape::core`, `shape::memory`
* **Configuration:** Initial capacity limits.
* **Serialization:** Saves component dense arrays as binary dumps.
* **Debugging:** Diagnostic tracking monitors active component index counts.
* **Testing:** Asserts that stale Entity handles generate invalid lookup flags.
* **Benchmarks:** Iteration speed profiling on dense arrays.
* **Performance Targets:** Amortized $O(1)$ accesses; cache line contiguous updates.
* **Future Extensions:** Dynamic sparse indices paging.

---

## 2. Verification & Validation (V&V)

* **Verification:** Unit tests assert that swap-and-pop updates maintain contiguous layouts in the dense array store on deletions.
* **Validation:** Long-running fuzzer scenarios verify that deferred CommandBuffers prevent iterator invalidation bugs when deleting entities during update runs.
