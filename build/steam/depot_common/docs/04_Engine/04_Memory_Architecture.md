# 04_Memory_Architecture

This document specifies the custom memory allocation systems, performance parameters, and zero-allocation runtime rules.

---

## 1. Transient Arena Allocator
The [ArenaAllocator](file:///C:/Users/ASUS/Documents/Primodilas/Engine/Memory/ArenaAllocator.hpp) manages transient, per-tick memory allocations:
* **Algorithm:** A single pre-allocated memory block with a bump pointer. Allocations increment the pointer:
  $$\text{Ptr}_{\text{new}} = \text{Ptr}_{\text{current}} + \text{Size}$$
* **Deallocation:** $O(1)$ complexity. Individual destructors are skipped; the pointer is reset back to the start of the block at the end of each tick.
* **Use Case:** Pathfinding calculations, temporary collision lists, and broadphase query buffers.

---

## 2. Block Pool Allocator
The [PoolAllocator](file:///C:/Users/ASUS/Documents/Primodilas/Engine/Memory/PoolAllocator.hpp) allocates fixed-size components:
* **Algorithm:** Links free memory blocks using a singly-linked list of pointers (free list).
* **Fragmentation:** Zero internal or external fragmentation.
* **Complexity:** $O(1)$ allocate and deallocate operations.

---

## 3. Zero-Allocation Runtime Contract
Simulation ticks must run without heap operations (System Law 5):
* standard containers (`std::vector`, `std::string`) are banned in tick updates unless pre-reserved or using custom arena allocators.
* Violating this rule triggers compile-time verification or runtime OOM reports.
