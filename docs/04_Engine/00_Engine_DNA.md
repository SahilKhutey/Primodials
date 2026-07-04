# 00_Engine_DNA

This document establishes the Meta-Architecture of the project — the universal design language, coding contracts, and architectural rules governing all frameworks and systems.

---

## 1. Naming Conventions

To maintain a consistent codebase across all subsystems:
* **Namespaces:** Lowercase singular nouns (e.g. `namespace shape::physics`, `namespace shape::ai`).
* **Systems:** Suffix `System` in PascalCase (e.g. `PhysicsSystem`, `CollisionSystem`).
* **Components:** Suffix `Component` in PascalCase (e.g. `RigidBodyComponent`, `MindComponent`).
* **Events:** Suffix `Event` in PascalCase (e.g. `CollisionEvent`, `DeathEvent`).

---

## 2. Dependency Rules

Layers follow a strict unidirectional hierarchy to prevent cyclic includes:
* Higher layers can reference lower layers but never vice versa.
* System classes must only communicate with other systems via the abstract `World` query interfaces or the asynchronous `EventQueue`.

---

## 3. Communication Patterns

Subsystems exchange state information through three distinct channels:

```
Direct Component Queries (Const, Read-only, Phase 2)
  │
  ├── CommandBuffer Mutations (Deferred execution, Phase 3)
  │
  └── Event Queue Broadcasts (Decoupled, Async, Next Tick)
```

* **Queries:** Systems run parallel read-only queries over components.
* **Commands:** Systems submit entity creation or deletion requests to a deferred `CommandBuffer`.
* **Events:** Systems publish event structs to the `EventQueue` to alert subscribers asynchronously on the next frame.

---

## 4. Lifecycle Model

Every simulation entity transitions through a uniform lifecycle managed by the ECS core:
1. **Initialize:** Entity index allocated, components populated.
2. **Update:** Systems iterate over component data buckets sequentially.
3. **Destruct:** Components freed back to pools, entity index returned to the free-list, decay resources spawned.

---

## 5. Concurrency Model

* **Synchronous Kernel:** The tick update loop runs sequentially on a single thread to guarantee state determinism.
* **Job System Work-Stealing:** Systems dispatch independent workloads (such as disjoint spatial grid collision checks) to a worker pool using work-stealing job queues.
* **Double-Buffered Render Sync:** Visual transform caches are extracted into a double buffer at the end of each tick, allowing the rendering thread to draw frames without locking the simulation.

---

## 6. Determinism Rules

* **Fixed Timestep:** Every tick increments the simulation clock by exactly $16666\mu\text{s}$. Real elapsed frame times are ignored during state calculations.
* **RNG Isolation:** Systems use a local `SimRNG` instance seeded from the initial world seed to prevent platform-specific floating-point or thread scheduling differences from causing desyncs.
