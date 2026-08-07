# 01_Engine_Architecture

This document serves as the authoritative engine specification, detailing modular interfaces, dependency boundaries, thread models, memory models, and execution schedules.

---

## 1. Engine Layers
The engine is structured in five execution layers. Modules in a layer may only call modules in the same layer or lower layers:

```
┌────────────────────────────────────────────────────────┐
│               Applications (Client Game)               │
├────────────────────────────────────────────────────────┤
│     Game Framework (HUD, camera, selection, inputs)    │
├────────────────────────────────────────────────────────┤
│   Simulation Framework (Time, clock, biology, career)  │
├────────────────────────────────────────────────────────┤
│        Engine Modules (ECS, math, custom physics)      │
├────────────────────────────────────────────────────────┤
│          Platform Layer (SDL3 OS insulation)           │
└────────────────────────────────────────────────────────┘
```

* **L0: Platform Layer:** Wraps OS windowing, hardware inputs polling, file system access, and network sockets.
* **L1: Engine Modules:** The core ECS registry, custom vector math libraries, and SAT collision solver.
* **L2: Simulation Framework:** Models biological lifecycles, genetics mutations, and Utility AI motivations.
* **L3: Game Framework:** Decouples camera transformations and unprojects viewport click events.
* **L4: Applications:** Bootstraps visual windows and loads specific scenarios (e.g. ALife Sandbox).

---

## 2. Module Directory Mappings
To enforce strict packaging boundaries, the codebase is partitioned into distinct subfolders:
* `/Engine/Core` — Compile-time basic types, custom error checks, and compiler macros.
* `/Engine/Platform` — SDL3 wrappers for keyboard, mouse, and thread pools.
* `/Engine/Memory` — custom Arena and block Pool allocators.
* `/Engine/Math` — Vector2, Shoelace area integration, and LCG pseudo-random states.
* `/Engine/ECS` — Dense-sparse array component stores and deferred CommandBuffers.
* `/Engine/Simulation` — Heartbeat clock schedulers and unified tick interfaces.
* `/Engine/Physics` — Euler-Cromer kinematics, spatial grid broadphase, and SAT collision solvers.
* `/Engine/Rendering` — Virtual draw wrappers, decoupled render graphs, and color maps.
* `/Engine/AI` — Saliency filters, short-term memory decays, and Utility AI.

---

## 3. Communication Rules
Modules communicate using three defined interfaces:
* **Direct Queries (Phase 2):** Systems query contiguous dense component arrays within sparse sets in read-only mode.
* **CommandBuffer Mutations (Phase 3):** Structural changes (creating/destroying entities, component modifications) are written to deferred buffers to prevent iterator invalidations during tick updates.
* **Event Queue Dispatch (Async):** Systems communicate asynchronously by pushing events to a decoupled `EventQueue`. Subscribers read these events on the next tick, avoiding circular system dependencies.

---

## 4. Memory Ownership Chain
The memory layout is structured hierarchically to eliminate runtime allocations:
* **Engine Scope:** Pre-allocates a large block of heap memory at initialization.
* **World Scope:** Owns the block Pool allocators for component registries.
* **Scene Scope:** Allocates static spatial grid partitions and route graphs.
* **Frame Scope:** The Arena allocator owns transient per-frame memory, resetting bump pointers at tick completion.

---

## 5. Simulation Execution Pipeline
Each simulation tick executes in a strict phase sequence:

```
[Phase 1: Input Poll] ──→ [Phase 2: System Ticks] ──→ [Phase 3: Write & Mutate]
```

1. **Phase 1: Input Poll:** Reads OS input packets, unprojects screen coordinates to world coordinates, and queues inputs.
2. **Phase 2: System Ticks:** Systems execute in sequence:
   $$\text{Physics} \rightarrow \text{Biology} \rightarrow \text{AI (Utility decisions)} \rightarrow \text{Careers (Skill growth)}$$
3. **Phase 3: Write & Mutate:** Flushes the CommandBuffer to apply structural updates and resets the transient Arena allocator.

---

## 6. Dependency Rules
* Modules in `/Engine` must never include headers from `/Runtime` or `/Examples`.
* Circular dependencies are prohibited: if module A includes module B, B must not include A. Forward class declarations and virtual base interfaces are used to break dependencies.

---

## 7. Threading Model
* **Simulation Thread:** Runs the tick scheduler sequentially on a single thread to guarantee deterministic state transitions.
* **Worker Threads:** Dispatches disjoint spatial hash grid regions to thread pools during physics broadphase calculations.
* **Render Thread:** Executes on a separate thread, reading simulation states via double-buffered visual transform caches to prevent rendering frames from stalling simulation loops.

---

## 8. Plugin Model
Systems are modular and pluggable:
* Renderer, physics, and AI systems inherit from abstract interfaces.
* New features are registered as modular ECS systems without modifying the core simulation loop.
* Scenario parameters and custom shape layouts are loaded via LuaJIT mod profiles at startup.

---

## 9. Error Handling & Recovery
* **Asserts:** Invariant rules (e.g. no negative mass or energy values) are enforced via `SHAPE_ASSERT` macros, triggering core dumps on failure.
* **Warnings:** Non-critical errors (e.g. memory arena approaching OOM thresholds) are logged asynchronously to prevent thread stalls.
* **Replay Recovery:** desyncs in network lockstep modes are corrected by rolling back to the last synchronized keyframe and re-simulating the frame history.

---

## 10. Testing Strategy
* **Catch2 Units:** Verifies individual module functionality (vector math, area calculations, pool allocation limits).
* **Fuzz Testing:** Simulates randomized high-density scenarios to verify stability.
* **desync Tests:** Runs two simulation threads with identical seeds, asserting matching CRC32 hashes each frame to catch non-deterministic compiler behaviors early.
