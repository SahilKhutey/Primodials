# Primodials (ShapeEngine)

**Primodials** is a high-performance, data-oriented C++23 simulation engine built around a custom Entity-Component-System (ECS) architecture. It is designed to simulate massively complex ecosystems, civilizations, and physics-driven entity interactions in a fully deterministic, headless environment.

## Features
- **Data-Oriented ECS**: Custom contiguous sparse sets and bump-allocated arenas for zero-runtime heap allocations.
- **Physics & Kinematics**: Euler-Cromer integration with Narrowphase SAT collision solvers and Broadphase Spatial Hash Grids.
- **Biology & Evolution**: Lotka-Volterra ecosystem dynamics, genetic crossover algorithms, and trait mutations.
- **AI & Cognition**: Utility AI, perception sweeps, dynamic memory log decay, and spatial influence fields.
- **Civilization & Economy**: Storage depot load balancing, technology tree propagation, and lockstep network synchronization.
- **Decoupled Architecture**: Strictly headless core mechanics allowing independent visualization layers and networking rollbacks.

## Project Structure
- `Engine/` - The core simulation library (ECS, Physics, Biology, AI, Math).
- `EngineTests/` - Catch2 unit test suite asserting determinism and physical laws.
- `Examples/` - Implementation examples integrating `ShapeEngine` with rendering backends (e.g. SDL3).
- `docs/` - Comprehensive technical architecture specifications and the Simulation Bible.

## Build Instructions (CMake)

The engine relies on a strict `-Werror` and `-std=c++23` compiler requirement (e.g., LLVM-MinGW/Clang).

```bash
mkdir build
cd build
cmake ..
cmake --build .
ctest -C Debug --output-on-failure
```

## Documentation
Please refer to the [Simulation Bible](docs/README.md) inside the `docs/` directory for exhaustive implementation details, subsystems, and the mathematical framework.

## License
This project is licensed under a Commercial Proprietary License. See the [LICENSE](LICENSE) file for details.
