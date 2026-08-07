# 21_Technology_Stack

This document establishes the official technology stack and modular boundaries for the project. Every engine subsystem, tool, build pipeline, and rendering driver must strictly adhere to this technical foundation.

---

## 1. Architectural Layers

The system is organized into decoupled layers where data and commands flow downward:

```
┌─────────────────────────────────────────────────────────┐
│              Application Layer (Client Game)            │
├─────────────────────────────────────────────────────────┤
│        Game Framework (HUD, input actions, camera)      │
├─────────────────────────────────────────────────────────┤
│    Simulation Framework (Clock, Biology, Career, AI)    │
├─────────────────────────────────────────────────────────┤
│      Engine Framework (ECS, Math, Custom Physics)       │
├─────────────────────────────────────────────────────────┤
│      Platform Framework (SDL3 OS wrapper, OpenAL)       │
└─────────────────────────────────────────────────────────┘
```

---

## 2. Core Specifications (28 Points)

### 1. Core Programming Languages
* **Primary Language:** C++23. Used for the entire runtime simulation pipeline, physics solvers, memory managers, and rendering engines to ensure predictable, deterministic performance.
* **Secondary Language:** Python 3.13+. Restricted to offline asset pipelines, dataset compilation, utility scripts, and automation test runners. Python is never executed inside the runtime thread.
* **Scripting Language:** LuaJIT. Employed for game scenarios config modifications and user mod files without recompiling the static binary. Planned migration: Custom visual scripting nodes.

### 2. Compiler
* **Primary Compiler:** Clang 18+. Selected for cross-platform diagnostic checks and static analysis sanitization.
* **Supported Toolchains:** MSVC (Windows), GCC (Linux), Clang (macOS).

### 3. Build System
* **CMake 3.28+** using the **Ninja** build generator for rapid, cross-platform build configurations.

### 4. Package Manager
* **vcpkg** (manifest mode) for resolving external libraries (SDL3, Catch2, OpenAL Soft) to maintain simple CMake compiler bindings.

### 5. Version Control
* **Git** hosted on GitHub. Large textures, binary sound assets, and pre-compiled dependencies are cataloged using **Git LFS** to prevent repository bloat.

### 6. IDE Mappings
* **Visual Studio 2022** (Windows primary), **CLion** (macOS/Linux secondary), **VS Code** (lightweight scripting).

### 7. Window & Platform Layer
* **SDL3:** Handles window state, keyboard/mouse coordinate polling, controller events, thread utilities, and bootstrapping graphics device contexts.

### 8. Graphics API
* **Phase 1:** OpenGL 4.6 (broad compatibility, simple setup).
* **Phase 2:** Vulkan (explicit control, multi-threaded command buffers).
* **Future Fallbacks:** DirectX 12 (Windows) / Metal (Apple). All APIs are decoupled using virtual base interfaces.

### 9. Rendering Architecture
* **Pipeline:** Common Renderer → Render Graph → Resource Cache → Pipeline State → GPU.
* **Features:** Instanced rendering for shapes, GPU-based culling, dynamic Level-of-Detail (LOD) meshes, and post-process glow shaders.

### 10. Mathematics Library
* **Custom Math:** A dedicated math module containing `Vector2`, `Vector3`, `Vector4`, `Matrix3`, `Matrix4`, `Quaternion`, and polygon geometry overlaps algorithms. The core simulation contains **zero third-party math dependencies** to guarantee cross-compiler determinism.

### 11. Memory System
* **Custom Allocators:** Standard runtime heap operations are forbidden during simulation loops. Memory is recycled using block `PoolAllocator` setups, and transient per-frame memory uses `ArenaAllocator` bump pointers.

### 12. ECS Framework
* **Custom ECS:** Implemented via dense-sparse array sets (`SparseSet<T>`) for $O(1)$ lookups and contiguous cache-coherent loop iterations. Defer deletions and insertions through a `CommandBuffer` flush.

### 13. Physics Engine
* **Custom 2D Physics:** Tailored for polygon SAT (Separating Axis Theorem) collision detection, Baumgarte stabilization positional corrections, and elastic impulse solvers.

### 14. Simulation Core
* **Core Subsystems:** Integrates Time (deterministic clocks), Biology (genetics expression), Careers (skill ranks), and Sentinels (memory loops) in a strict execution order.

### 15. Artificial Intelligence
* **Layered AI:** Combines Utility AI (action evaluations), Finite State Machines (low-level steering), and Influence Maps (attractors/repellers).

### 16. Pathfinding
* **Algorithms:** A* for local grids, Flow Fields for high-count swarms, and Hierarchical Pathfinding for inter-colony routes.

### 17. Parallelism
* **Job System:** Exploits AVX2 SIMD registers for vector calculations and maps regional spatial chunks to thread pools via task graphs.

### 18. Serialization
* **Formats:** Bounded binary format for runtime save snapshots and deterministic input packet streams for replay re-simulations.

### 19. Asset Pipeline
* **Assets Compiler:** Pre-compiles Outfit TTF fonts, WAV/OGG audio files, and JSON scenario files into optimized binary formats.

### 20. User Interface
* **Dev UI:** Dear ImGui.
* **Player UI:** Custom retained-mode UI widgets decoupled from the simulation.

### 21. Audio Engine
* **OpenAL Soft** for spatial 3D audio, fading sounds using distance decay curves.

### 22. Networking
* **Network Protocol:** Deterministic lockstep with rollback and input delay buffers, syncing peer states via CRC32 hashes.

### 23. Testing Framework
* **Catch2 v3.4+** executing unit, integration, and fuzz tests.

### 24. Profiling & Debugging
* **Tools:** Tracy (CPU/GPU profile loops), AddressSanitizer (leak checks), and clang-tidy (code styling checkmarks).

### 25. Continuous Integration (CI)
* **GitHub Actions:** Runs compiler build verification, unit tests, static analyzers, and compiles releases.

### 26. GPU Compute
* **Accelerators:** OpenGL Compute Shaders transitioning to Vulkan Compute for accelerating high-count fluid chemistry diffusions.

### 27. Documentation
* **Markdown and plantUML** files kept under version control, treating design documentation as code.

### 28. Repository Directory Structure
The repository matches the following clean directory mapping:
* `/Engine` — Static simulation core library.
* `/Editor` — ImGui overlays and sandboxing tools.
* `/Runtime` — Executable build entry points.
* `/Simulation` — World, Clock, and Event systems.
* `/Game` — Visual game client logic ("Polygonal Primordials").
* `/Tools` — Offline Python converters and pipeline scripts.
* `/ThirdParty` — External library configuration wrappers.
* `/Assets` — Fonts, sounds, shaders.
* `/Tests` — Catch2 verification test files.
* `/Benchmarks` — Profiling scripts and execution logs.
* `/Documentation` — The 5 volumes of the Simulation Bible.
* `/Examples` — Simple test applications.
* `/Build` — Local compilation outputs.
* `/Scripts` — Automation files and compiler batch scripts.

---

## 3. Technology Guiding Principles
* **Deterministic:** State updates must depend exclusively on input parameters and seeds.
* **Modular:** Modules communicate via clean Interfaces, allowing rendering or physics backends to be swapped out easily.
* **Data-Oriented:** Data is stored contiguously, optimizing CPU cache locality.
* **Observable:** Internal values are transparent and inspectable in real-time.
