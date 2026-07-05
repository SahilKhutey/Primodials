# Polygonal Primordials & Shape Engine 

Welcome to the **Polygonal Primordials** repository! This project consists of two major components:
1. **Shape Engine**: A production-grade, highly-deterministic Entity Component System (ECS) C++23 game engine built from scratch.
2. **Polygonal Primordials**: A deep artificial life simulation and evolution sandbox powered by the Shape Engine.

## Project Phases Completed
This repository represents the culmination of a massive, multi-phase engineering effort spanning eight distinct phases:

- **Phase A (Foundation):** Core mathematics, SIMD vectors, custom memory allocators (Pool, Arena), lock-free data structures, and the foundational `ShapeConfig` & CMake build system.
- **Phase B (ECS & Platform):** High-performance Archetype-based ECS capable of simulating 1,000,000 entities at 60 FPS. Integration with SDL3 for cross-platform windowing and input handling.
- **Phase C (Simulation):** Deterministic tick scheduler (60Hz fixed-step), spatial hashing, and the complex Species Schema powering biological and genetic simulation.
- **Phase D (Evolution):** Emergent gameplay mechanics including reproduction, genetic crossover, mutation, combat, predator-prey dynamics, and dynamic speciation.
- **Phase E (Editor & Replay):** Full Dear ImGui integration, providing a professional hierarchy, inspector, and profiler. Complete deterministic rollback and replay timeline scrubbing.
- **Phase F (Multiplayer & Vulkan):** Lockstep network multiplayer with rollback netcode. Advanced graphics pipelines preparing the groundwork for Vulkan rendering and Lua modding integrations.
- **Phase G (Shipping):** Steamworks API integration, telemetry, automated crash reporting, accessibility features, localization, and a comprehensive build cooker.
- **Phase H (LiveOps):** Post-launch support infrastructure including a real-time dashboard, anomaly detection, cohort analysis, Discord/Steam moderation bots, and modding Workshop curation tools.

## Repository Structure

```
Primodilas/
├── CMakeLists.txt           # Master CMake configuration
├── Engine/                  # Core Shape Engine (Math, Memory, ECS, Platform)
├── Editor/                  # Dear ImGui-based visual editor
├── Games/                   # Target applications
│   └── PolygonalPrimordials/# The main artificial life simulation game
├── LiveOps/                 # Live-service telemetry, A/B testing, and health dashboards
├── Bots/                    # Discord and Steam Moderation automation
├── Content/                 # Lua scripts, JSON schemas, and game configurations (Mods)
├── Tools/                   # Asset cooker, docs generator, trailer renderer, and liveops CLI
├── Marketing/               # Launch assets, trailers, and announcement templates
└── docs/                    # Extensive runbooks, manuals, and design documents
```

## Building the Engine

### Prerequisites
- **Compiler**: A C++23 compliant compiler (MSVC 19.34+, GCC 13+, Clang 16+).
- **CMake**: Version 3.22 or higher.
- **Git**: Required for FetchContent dependencies (SDL3, Catch2).

### Build Instructions
```bash
# Clone the repository
git clone <YOUR_GITHUB_REPO_URL>
cd Primodilas

# Configure the build (Out-of-source)
cmake -S . -B build -D CMAKE_BUILD_TYPE=Release

# Build the project
cmake --build build --config Release
```

## Running the Game & Tools
After building, executables will be located in the `build/` directory (or `build/Release/` on MSVC).

- **Game**: `PolygonalPrimordials.exe`
- **Editor**: `ShapeEditor.exe`
- **Discord Bot**: `DiscordBot.exe`
- **Dashboard API**: Runs internally via the LiveOps server module.

## Modding & Workshop
Polygonal Primordials supports extensive community modifications via Lua and JSON schemas.
Check the `Content/v1.2_modding/` directory for templates.

## Architecture Guidelines
- **Determinism**: Cross-platform determinism is the lifeblood of this engine. Do not use standard floating-point functions (`std::sin`, `std::sqrt`) or standard random generators (`std::rand`) in the simulation layer. Always use `Shape::Math`.
- **Memory**: Avoid `new`/`delete`. Use `LinearAllocator`, `PoolAllocator`, or standard library containers utilizing custom allocators where possible.
- **Testing**: Every new system must have comprehensive unit tests in `EngineTests/` to maintain the stability of the rollback netcode.

---
*Built with passion. Welcome to the primordial soup.*
