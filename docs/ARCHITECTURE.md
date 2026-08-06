# Architecture Overview

Deep dive into Polygonal Primordials' system design.

---

## 🎯 Design Goals

1. **Performance**: Handle 250K entities @ 60 FPS
2. **Determinism**: Reproducible simulation (critical for lockstep replays)
3. **Modularity**: Easy to extend and modify
4. **Portability**: Run on Windows, Linux, macOS identically
5. **Data-Oriented**: Cache-friendly, minimal allocations
6. **Open Source**: MIT licensed, learnable codebase

---

## 🏗️ High-Level Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                       Application Layer                     │
│  ├─ Game Mode (full UI, simulation controls, editor)        │
│  └─ Wallpaper Mode (WorkerW window, ambient controls)       │
├─────────────────────────────────────────────────────────────┤
│                         Engine Systems                      │
│  ├─ ECS2 (Archetype-based Entity Component System)          │
│  ├─ Spatial Hash Broadphase (O(1) neighbor queries)         │
│  ├─ Renderer (SDL3 Renderer & OpenGL 4.6 backend)           │
│  ├─ Audio (SDL_mixer ambient player wrapper)               │
│  ├─ Scripting (Lua 5.4 sandboxed environment)              │
│  └─ Steam Integration (Workshop, Cloud, Achievements)       │
├─────────────────────────────────────────────────────────────┤
│                         Core Libraries                      │
│   SDL3  │  Dear ImGui  │  GLM  │  Sol2  │  zstd  │  Lua     │
└─────────────────────────────────────────────────────────────┘
```

---

## 🧩 Core Systems

### 1. Archetype Entity Component System (`ECS2`)
Entities are compact numerical IDs (`EntityId`). Components are flat C++ structs registered in `ComponentRegistry`. Entities sharing identical component sets reside in dense `Archetype` memory chunks, yielding ~1ns per entity cache iteration.

### 2. Spatial Broadphase
Uses uniform 50x50 spatial hash grids to resolve neighbor queries in O(1) time instead of O(N²), keeping 250,000 entity interactions performing at 60 FPS.

### 3. Rendering Pipeline
Uses instanced draw calls to push 250,000 entities in 1 draw call to GPU VRAM, combined with camera frustum culling.

### 4. Deterministic Simulation
Uses deterministic fixed 60Hz tick rates and bit-exact pseudo-random number generators (`XorShiftRNG`) for repeatable evolution trajectories.

---

## 🔌 Extension Points

- **New Component**: Register struct in `ComponentRegistry::register_type<T>()`.
- **New Theme**: Create JSON file in `Content/Themes/<name>/theme.json`.
- **New Lua Mod**: Add script in `Content/Mods/<name>/scripts/main.lua`.
