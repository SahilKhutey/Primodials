# Polygonal Primordials & Shape Engine

[![C/C++ CI Matrix](https://github.com/SahilKhutey/Primodials/actions/workflows/ci.yml/badge.svg)](https://github.com/SahilKhutey/Primodials/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![C++23 Standard](https://img.shields.io/badge/C%2B%2B-23-blue.svg)](https://en.cppreference.com/w/cpp/23)

Welcome to **Polygonal Primordials & Shape Engine** — a high-performance, deterministic C++23 simulation platform and artificial life sandbox.

---

## 1. Release Status & Milestone Breakdown

### v2.0 (Current Released)
- ✅ **Archetype-Based ECS (ECS2)**: Data-oriented SoA layout designed for 100,000+ entity simulations.
- ✅ **OpenGL 4.6 Instanced Renderer**: Hardware instanced rendering pipeline (`DrawInstancedPolygons`, `DrawInstancedCircles`) using SDL3 GL context.
- ✅ **Deterministic Math Layer**: Fixed-step 60Hz tick scheduler with cross-platform floating point determinism (`Shape::Math::Sin`, `Shape::Math::Sqrt`, `DeterministicRng`).
- ✅ **2D Orthographic Camera**: Pan/zoom camera matrix transformation system (`CameraController`).
- ✅ **Biological & Genetics Simulation**: Genetic crossover, mutation, combat, predator-prey dynamics, and speciation.
- ✅ **Comprehensive Test Suite**: Catch2 v3.4.0 test harness with 186 unit and integration test cases.
- ✅ **Cross-Platform CI Matrix**: GitHub Actions testing MSVC, GCC, and Clang on Windows and Linux runners.

### v2.1 Roadmap (Planned Q4 2026)
- ⏳ **ImGui Docking Visual Editor**: Hierarchy, inspector, live stats, and real-time population graph panels.
- ⏳ **Templated Spatial Hash Grid**: Uniform grid broadphase for $O(1)$ neighbor predator-prey queries.
- ⏳ **Lua Sandboxing Engine**: CPU instruction-count hooks, 64 MB memory limits, and API whitelisting.

### v3.0 Roadmap (Planned 2027)
- ⏳ **Vulkan Renderer Backend**: Low-overhead Vulkan rendering pipeline.
- ⏳ **Steamworks Integration & LiveOps**: Telemetry dashboards and Steam Workshop modding loader.
- ⏳ **Multiplayer Matchmaking**: Rollback lockstep network server implementation.

---

## 2. Building the Engine

### Prerequisites
- **Compiler**: C++23 compliant compiler (MSVC 19.34+, GCC 13+, or LLVM-MinGW Clang 16+)
- **Build System**: CMake 3.25+ and Ninja
- **Dependencies**: Auto-fetched via CMake (`SDL3`, `Catch2`)

### Build Steps
```bash
# Clone the repository
git clone https://github.com/SahilKhutey/Primodials.git
cd Primodials

# Configure build
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release

# Compile Engine, Standalone Game, and Tests
cmake --build build --config Release

# Run test suite
./build/bin/ShapeEngineTests.exe
```

---

## 3. Documentation & Governance

- 📐 **[Architecture Guide](docs/ARCHITECTURE.md)**: System layout, memory allocators, and determinism rules.
- 🤝 **[Contributing Guidelines](CONTRIBUTING.md)**: Dev setup, coding style, and PR workflow.
- 📜 **[Code of Conduct](CODE_OF_CONDUCT.md)**: Contributor Covenant v2.1.
- 📄 **[License](LICENSE)**: Open-source MIT License.

---
*Built with passion. Welcome to the primordial soup.*
