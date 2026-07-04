# DataStructures

This document specifies the core data layouts, structural alignments, and memory ownership constraints.

---

## 1. Components Layout & Alignment
All component structures are designed as plain data structs (zero virtual methods) aligned to CPU cache lines (64 bytes):

```cpp
struct alignas(64) RigidBodyComponent {
    Vector2 position;         // 8 bytes
    Vector2 velocity;         // 8 bytes
    f32     angle;            // 4 bytes
    f32     angularVelocity;  // 4 bytes
    f32     mass;             // 4 bytes
    f32     invMass;          // 4 bytes
    f32     inertia;          // 4 bytes
    f32     invInertia;       // 4 bytes
    f32     restitution;      // 4 bytes
    f32     drag;             // 4 bytes
    Vector2 forceAccum;       // 8 bytes
    f32     torqueAccum;      // 4 bytes
    u32     padding[2];       // 8 bytes padding to fill 64 bytes
};
```

---

## 2. Memory Ownership Constraints
* **Sparse Set Ownership:** Component stores own their contiguous dense arrays.
* **Entity Handles:** Entities do not own data; they are lightweight 32-bit index indices.
* **Allocators Ownership:** The Arena allocator owns all transient per-tick memory structures.

---

## 3. Serialization Contracts
* Structures must be trivially constructible and copyable.
* File serialization writes components directly as raw binary chunks (`std::ofstream::write`), avoiding string parsing overhead during save operations.
