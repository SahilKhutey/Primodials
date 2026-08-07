# 01_Entity_System

This document specifies the Entity identity model — how entities are created, recycled, identified, and destroyed throughout the simulation lifecycle.

---

## 1. Entity Identity Model

An `Entity` is not an object. It is a lightweight 32-bit handle that serves as a key into component stores.

```
Entity (u32)
 ┌─────────────────────────┬──────────────────┐
 │  Index (bits 0–19)      │ Generation (20–31)│
 └─────────────────────────┴──────────────────┘
       20 bits                    12 bits
  Max 1,048,576 entities      4,096 generations
```

* **Index:** Points to the dense array slot within each component store.
* **Generation:** A monotonic counter incremented every time a slot is recycled. Stale handles referencing old generation values are automatically detected as invalid.

**C++ Symbol:** [Entity.hpp](file:///C:/Users/ASUS/Documents/Primodilas/Engine/ECS/Entity.hpp)

---

## 2. Lifecycle States

Every entity passes through a defined set of lifecycle states:

```
PENDING_CREATE
      │
      ▼
    ALIVE ◄──────────────┐
      │                   │ (Revive from dormant)
      ├──► DORMANT ───────┘
      │
      ▼
 PENDING_DESTROY
      │
      ▼
  DESTROYED → slot recycled → generation incremented
```

* **ALIVE:** Participating in all system queries.
* **DORMANT:** Suspended from physics and AI updates (used for LOD distance culling). State is preserved.
* **PENDING_DESTROY:** Flagged for destruction after the current tick's `CommandBuffer` flush.
* **DESTROYED:** Slot is freed and pushed onto the free-list for recycling.

---

## 3. Entity Creation & Recycling

**Creation Algorithm:**
```
if freeList is not empty:
    index = freeList.pop()
    generation[index] += 1
    return Entity(index, generation[index])
else:
    index = nextIndex++
    generation[index] = 0
    return Entity(index, 0)
```

**Validation:**
```cpp
bool IsValid(Entity e) {
    return generation[e.Index()] == e.Generation();
}
```

All creation and destruction requests are submitted through the `CommandBuffer` and flushed **after** all systems complete their update pass (see [19_System_Laws](file:///C:/Users/ASUS/Documents/Primodilas/docs/01_Foundations/19_System_Laws.md), Law 3).

---

## 4. Entity Archetypes

Archetypes group common component combinations for fast batch creation. Pre-defined archetypes include:

| Archetype     | Components                                                              |
|--------------|-------------------------------------------------------------------------|
| `BasicShape`  | RigidBody, Geometry, Lifecycle, Genetics, AI, Mind, Career             |
| `Sentinel`    | RigidBody, Geometry, Lifecycle, Sentinel, Influence                    |
| `ResourceNode`| RigidBody, Geometry, Resource                                           |
| `Building`    | RigidBody, Geometry, Lifecycle, Storage, Influence                     |

Archetypes are templates — they define the component layout but carry no behavior themselves.

---

## 5. Entity Capacity & Limits

| Parameter          | Value         |
|-------------------|---------------|
| Max simultaneous entities | 1,048,576 |
| Max generations per slot  | 4,096     |
| Free list capacity        | Dynamic   |
| Dormant entity limit      | Configurable per region |

When the entity limit is approached, the simulation triggers an automatic carrying-capacity pressure response: resource scarcity increases, mortality rates rise, and reproduction utility falls.
