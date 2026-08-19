# 🧠 PRIMORDIALS — Core Engine & ECS Architectural Enhancements

---

## 1. Structure-of-Arrays (SoA) Component Layout

### Problem (Array-of-Structures)
```cpp
struct Entity {
    Position pos;       // 8 bytes
    Velocity vel;       // 8 bytes
    Species species;    // 32 bytes
}; // Total: 48 bytes
// Traversing positions alone pollutes L1 cache with unused velocity and species data.
```

### Solution (Archetype SoA)
```cpp
struct ArchetypeChunk {
    alignas(64) std::array<float, 64> pos_x;
    alignas(64) std::array<float, 64> pos_y;
    alignas(64) std::array<float, 64> vel_dx;
    alignas(64) std::array<float, 64> vel_dy;
};
// 100% cache-line utilization during spatial updates.
```

---

## 2. Generational Entity Handles (ABA Protection)

```cpp
class Entity {
    uint32_t id_ : 20;           // Up to 1,048,576 concurrent entities
    uint32_t generation_ : 12;   // Up to 4,096 recycle generations
};
```
Eliminates dangling reference bugs when entities are recycled across frames.

---

## 3. C++23 PMR Arena Allocators

By binding `std::pmr::monotonic_buffer_resource` to per-frame temporary systems, heap allocation overhead drops to **zero** during standard simulation ticks.
