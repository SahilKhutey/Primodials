# Architecture

This document specifies the core engine architecture, design patterns, and layering boundaries.

---

## 1. Modular Execution Boundaries
The engine uses a strict **unidirectional dependency pipeline**. Systems at higher levels execute operations and queries on lower layers, but lower layers have no awareness of higher-level structures:

```
┌────────────────────────────────────────────────────────┐
│             Application (Polygonal Primordials)        │
├────────────────────────────────────────────────────────┤
│         Game Framework (HUD, camera, selection)        │
├────────────────────────────────────────────────────────┤
│    Simulation Kernel (Ticks scheduler, Event Queue)    │
├────────────────────────────────────────────────────────┤
│      Engine Core (Sparse Set ECS, custom math)         │
├────────────────────────────────────────────────────────┤
│         Platform Wrapper (SDL3 OS insulation)          │
└────────────────────────────────────────────────────────┘
```

---

## 2. Monorepo Directory Boundaries
To prevent cycle dependencies during builds, source files are strictly partitioned into modules:
* `/Engine` — Static library core compiling math, registries, and simulation systems.
* `/Runtime` — Client executable bootstrapping platform assets and window loops.
* `/Tests` — Catch2 verification units.

---

## 3. Dynamic Execution Threading
To support high entity scale while preserving determinism:
* The simulation tick loop executes sequentially on a single thread.
* Multi-threading is localized: disjoint spatial grid segments are dispatched to worker thread pools during physics sweeps, ensuring thread-safe data updates.
