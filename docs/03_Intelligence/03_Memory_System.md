# 03_Memory_System

This document specifies the memory logging system, decay rates, and memory storage layouts.

---

## 1. Memory Storage Layout
Every entity maintains a localized memory ring buffer inside its [MindComponent](file:///C:/Users/ASUS/Documents/Primodilas/Engine/ECS/MindComponent.hpp):

```cpp
struct MemoryLog {
    Vector2    position;   // Resource/threat coordinate
    f32        strength;   // Importance value (0.0 to 100.0)
    f32        age;        // Time in seconds since observation
    MemoryType type;       // Resource, Threat, Ally
};
```

---

## 2. Memory Decay Mechanics
Memory strength decays exponentially over elapsed time to model forgetting and coordinate obsolescence:

$$\text{strength}_{t+1} = \text{strength}_t \times e^{-\lambda \cdot \Delta t}$$

* **Decay Rate ($\lambda$):** Inversely proportional to the entity's Science affinity gene. Entities with high Science interest remember coordinates significantly longer.
* **Eviction:** When $\text{strength}$ falls below $1.0$, the memory entry is evicted from the ring buffer.

---

## 3. Memory Reinforcement
If an entity re-observes an coordinate already present in its memory log, the entry is updated:
* The coordinate position is corrected to the current observed location.
* $\text{strength}$ is reset to $100.0$.
* $\text{age}$ is reset to $0.0$.
This prevents shapes from navigating to exhausted resource nodes.
