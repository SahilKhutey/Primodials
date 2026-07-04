# 00_Engine_Architecture_Blueprint

This document serves as the primary system specification, outlining the architectural guidelines, execution constraints, and coding contracts for the project.

---

## 1. Engine Philosophy
The engine is built around a single unifying mandate: **The simulation is the primary product; rendering is simply one way to observe it.**
* **Deterministic Execution:** The engine guarantees bit-identical output across target architectures (Windows, Linux, macOS). Non-deterministic float operations are eliminated using static compile-time switches.
* **Emergence over Scripting:** Features and behavioral states are not hardcoded. Instead, complex dynamics emerge from local needs, physics vectors, and utility AI evaluations.
* **Centralization of State:** All dynamic variables are stored in ECS components. No hidden local states are maintained inside systems.

---

## 2. Module Decomposition & Dependencies
The engine enforces a strict downward-only dependency hierarchy:

```
┌────────────────────────────────────────────────────────┐
│            Game Layer (Polygonal Primordials)          │
├────────────────────────────────────────────────────────┤
│           Simulation Kernel (Clock, Ticks)             │
├────────────────────────────────────────────────────────┤
│       Engine core (ECS Sparse Sets, Custom Math)       │
├────────────────────────────────────────────────────────┤
│    Platform Wrapper (SDL3 Windowing, inputs polling)   │
└────────────────────────────────────────────────────────┘
```
Systems at a higher layer query and modify lower layers but never vice versa. Upward method calls are strictly forbidden.

---

## 3. Public APIs & Module Contracts
To preserve modularity:
* Every module exposes its public interface in clean header files located under its namespace directories (e.g., `Engine/ECS/World.hpp`).
* Interfaces utilize forward declarations and reference bindings to insulate modules from compiler changes, keeping include scopes narrow.

---

## 4. Data Flow & Simulation Flow
The simulation frame runs in a strict sequential loop (the Tick Scheduler):

```
Tick Start ──→ [Phase 1: Input Poll] ──→ [Phase 2: System Ticks] ──→ [Phase 3: Write & Mutate] ──→ Tick End
```

* **Phase 1: Input Poll:** Inputs are translated into event packets and pushed to the `EventQueue`.
* **Phase 2: System Ticks:** Systems execute in order (Physics → Influence → Biology → Cognition → Careers). Structural changes (create/destroy entity) are deferred to prevent iterator invalidations.
* **Phase 3: Write & Mutate:** The `CommandBuffer` is flushed to apply deferred modifications, and the transient memory Arena is reset.

---

## 5. Threading Model
The simulation updates synchronously on a single logical thread to ensure determinism. Parallelization is handled at two scales:
* **Spatial Job Dispatching:** Independent spatial hash regions are split across CPU cores using thread pools.
* **Headless Decoupling:** Rendering and graphics drawing updates execute on a separate thread, reading data via interpolated double-buffers.

---

## 6. Memory Model
* **Pool Allocator:** Component allocations recycle fixed-size memory slots in contiguous arrays, eliminating fragmentation.
* **Arena Allocator:** Transient calculations (pathfinding, local queries) use a bump-pointer block with $O(1)$ allocation and bulk reset.
* **Allocation Invariant:** Heap allocation operations (`new`, `malloc`) are forbidden inside simulation ticks.

---

## 7. Build System Layout
* **CMake & Ninja:** CMake handles package dependencies (via vcpkg in manifest mode) and compiles targets into a static library (`ShapeEngine`) and verification runners (`ShapeEngineTests`).

---

## 8. Coding Standards
* **Style:** Enforced via clang-format guidelines.
* **Naming:** PascalCase for Classes and Methods; camelCase for variables; UPPER_CASE for macros.
* **Safety:** No dynamic casts (use static cast checks instead); references are preferred over raw pointers.

---

## 9. Extension / Plugin Model
The engine supports compilation plugins:
* Features are registered as custom ECS systems.
* Modding scripts use LuaJIT to modify scenario JSON maps and base resource properties without recompiling the executable.

---

## 10. Testing & Benchmarking Strategy
* **Catch2 Framework:** Unit and integration tests run verified scenarios against fixed seeds.
* **Invariance Testing:** Long-running fuzzer simulations verify energy conservation ($\Delta E = 0.0$) and mass preservation invariants.
* **Profiling:** CPU cache line hit rates and system tick timings are tracked via Tracy diagnostics.
