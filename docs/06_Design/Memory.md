# Memory

This document specifies the custom Arena Allocator, Pool Allocator, and zero runtime allocation constraints.

---

## 1. Transient Arena Allocator
* **Algorithm:** Allocates memory contiguously via a bump pointer from a pre-reserved block.
* **Complexity:** $O(1)$ allocations; individual object deallocations are skipped.
* **Reset:** The bump pointer is reset back to the start of the block at the end of each tick, recycling the entire buffer.

---

## 2. Block Pool Allocator
* **Algorithm:** Manages a linked list of free blocks (free list) within a pre-allocated segment.
* **Use Case:** Component allocation within ECS registry buckets.
* **Fragmentation:** Zero internal or external fragmentation.

---

## 3. Zero Runtime Allocation Invariant
No system may trigger heap operations (`new`, `malloc`, or dynamic container resizes) during a simulation update tick:
* All collection structures (`std::vector`, etc.) must pre-allocate using `reserve()` or utilize the Arena Allocator.
* Violating this rule triggers compile-time warnings or runtime OOM reports.
