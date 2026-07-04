# 03_World_System

This document specifies the `World` — the central ECS registry coordinating all entities, component stores, systems, and tick scheduling.

---

## 1. World Responsibilities

The `World` is the top-level simulation object. It owns:

```
World
 ├── EntityRegistry         — entity creation, recycling, validation
 ├── ComponentStores        — one SparseSet<T> per component type
 ├── CommandBuffer          — deferred structural mutations
 ├── SystemScheduler        — ordered system update calls
 ├── SpatialHashGrid        — broadphase spatial queries
 ├── SimulationClock        — fixed timestep accumulator
 ├── SimRNG                 — deterministic random number generator
 └── EventQueue             — simulation event bus
```

**C++ Symbol:** [World.hpp](file:///C:/Users/ASUS/Documents/Primodilas/Engine/ECS/World.hpp)

---

## 2. Tick Execution Order

Every tick follows this exact ordering. Systems at a lower layer must complete before the next layer begins (see [19_System_Laws](file:///C:/Users/ASUS/Documents/Primodilas/docs/01_Foundations/19_System_Laws.md), Law 1):

```
Tick N
 │
 ├── 1. Advance SimulationClock
 ├── 2. Flush EventQueue (previous tick's events)
 ├── 3. PhysicsSystem::Update()
 ├── 4. InfluenceSystem::Update()
 ├── 5. BiologySystem::Update()
 ├── 6. CognitiveSystem::Update()
 ├── 7. OptimizationSystem::Update()    ← AI decisions
 ├── 8. CareerSystem::Update()
 ├── 9. SentinelSystem::Update()
 ├── 10. Interaction resolution
 ├── 11. Statistics collection
 └── 12. CommandBuffer::Flush()         ← deferred mutations applied
```

This ordering ensures that physics runs before cognition, cognition runs before decisions, and all structural mutations are deferred safely.

---

## 3. World Query Interface

```cpp
// Create entity (deferred)
Entity e = world.CreateEntity();                    // immediate (pre-tick only) or deferred

// Add component
world.AddComponent<RigidBodyComponent>(e, rb);

// Get component reference
auto& rb = world.GetComponent<RigidBodyComponent>(e);

// Check if component exists
bool has = world.HasComponent<MindComponent>(e);

// Multi-component query view
auto view = world.Query<AIComponent, RigidBodyComponent>();
for (Entity entity : view) { /* ... */ }
```

---

## 4. World Initialization Sequence

On simulation start:
1. Load or generate world seed → seed `SimRNG`.
2. Allocate Arena and Pool allocator blocks.
3. Register all component stores.
4. Spawn initial entity archetypes from the scenario definition.
5. Begin tick loop.

---

## 5. World Serialization (Save / Load)

A complete world snapshot stores:
* All entity index + generation pairs.
* All component arrays per type (raw binary blobs).
* `SimRNG` state.
* `SimulationClock` tick counter.
* `EventQueue` pending events.

This allows exact state restoration for replay, network resynchronization, or save-file loading.
