# 06_AI_Architecture

This document specifies the Utility AI system, query iteration architectures, and tick budgets.

---

## 1. Contiguous Query Iterations
The [OptimizationSystem](file:///C:/Users/ASUS/Documents/Primodilas/Engine/AI/OptimizationSystem.cpp) processes decision logic in linear loops to maximize cache performance:
* The system queries all entities possessing `AIComponent`, `RigidBodyComponent`, and `LifecycleComponent`.
* Component values are processed sequentially inside the contiguous dense array blocks, avoiding cache misses.

---

## 2. Allocation-Free Intention Stacks
Goal stacks use fixed-size static arrays to prevent dynamic heap allocations:
```cpp
struct AIComponent {
    AIAction goalStack[4];      // Bounded stack depth
    u32      stackDepth = 0;    // Current depth index
    // ...
};
```
Push and pop operations adjust the index pointer in $O(1)$ complexity with zero memory allocation.

---

## 3. Tick Budget Distribution
AI evaluation scales dynamically to maintain target framerates (e.g. 60Hz):
* **Tick Decoupling:** Only a subset of active shapes update their utility scores each tick, spread across frames.
* **Proximity Prioritization:** Shapes closest to the camera or player are updated at higher frequencies than distant shapes.
