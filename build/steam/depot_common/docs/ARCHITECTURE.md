# ShapeEngine Architecture & Design Overview

ShapeEngine is a high-performance, cross-platform, deterministic 2D simulation engine built in C++23. It is designed specifically for data-oriented artificial life simulations, predator-prey dynamics, and lockstep network execution.

---

## 1. Engine Layer Layout

```
                  ┌─────────────────────────────────────┐
                  │    Games / PolygonalPrimordials     │
                  └──────────────────┬──────────────────┘
                                     │
      ┌──────────────────────────────┼──────────────────────────────┐
      │                              │                              │
┌─────▼──────────┐         ┌─────────▼────────┐           ┌─────────▼────────┐
│ Simulation Core│         │ Rendering System │           │ Network / Replay │
│ (Genetics/Grid)│         │ (OpenGL 4.6 / GL)│           │ (Lockstep/Sync)  │
└─────┬──────────┘         └─────────┬────────┘           └─────────┬────────┘
      │                              │                              │
      └──────────────────────────────┼──────────────────────────────┘
                                     │
                    ┌────────────────▼────────────────┐
                    │      Archetype ECS (ECS2)       │
                    └────────────────┬────────────────┘
                                     │
                    ┌────────────────▼────────────────┐
                    │ Memory Allocators & Shape::Math │
                    └─────────────────────────────────┘
```

---

## 2. Core Subsystems

### A. Memory Management (`Engine/Memory/`)
- **ArenaAllocator**: Linear bump allocator for frame-temporary allocations (zero heap fragmentation).
- **PoolAllocator**: Fixed-size block allocator for chunked archetype component storage.

### B. Deterministic Math Layer (`Engine/Math/`)
- Replaces standard `<cmath>` math library with deterministic floating-point operations (`Shape::Math::Sin`, `Shape::Math::Cos`, `Shape::Math::Sqrt`).
- Enforces strict floating-point rounding modes across GCC, Clang, and MSVC compiler environments (`-fno-fast-math`, `/fp:strict`).

### C. Archetype-Based ECS (`Engine/ECS2/`)
- **Cache-Coherent SoA Layout**: Entity components are packed tightly in contiguous memory chunks grouped by Archetype.
- **Scalability**: Designed for high entity iterations (100,000+ entities) with minimal CPU cache misses.

### D. High-Density Renderer Backend (`Engine/Rendering/`)
- **`OpenGLRenderer`**: Hardware instanced rendering pipeline (`DrawInstancedPolygons`, `DrawInstancedCircles`) using SDL3 GL core context.
- **`CameraController`**: 2D orthographic camera view/projection matrix generation for pan and zoom interactions.

---

## 3. Determinism Contract & Rules

1. **Randomness**: Use `Shape::Simulation::DeterministicRng` initialized with a fixed seed. Never call `rand()` or `std::rand()`.
2. **Timing**: Fixed-delta timestep execution via `SimulationClock` (60 Hz tick rate).
3. **Sorting**: Use `std::stable_sort` on all entity queries and fitness evaluations to guarantee identical order across compilers.
