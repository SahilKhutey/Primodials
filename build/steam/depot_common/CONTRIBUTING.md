# Contributing to ShapeEngine & Polygonal Primordials

Thank you for your interest in contributing to **ShapeEngine**! We welcome contributions from developers, engine architects, and open-source enthusiasts.

---

## 1. Code of Conduct

All contributors are expected to uphold our [Code of Conduct](CODE_OF_CONDUCT.md) in all interactions and communications.

---

## 2. Getting Started

### Prerequisites
- **C++ Compiler**: C++23 compliant compiler (LLVM-MinGW Clang 16+, GCC 13+, or MSVC 19.34+)
- **Build System**: CMake 3.25+ and Ninja
- **Python**: Python 3.10+ (for helper scripts)

### Building the Project
```bash
# Clone the repository
git clone https://github.com/SahilKhutey/Primodials.git
cd Primodials

# Configure build directory
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug

# Build engine and test suite
cmake --build build

# Run unit tests
./build/bin/ShapeEngineTests.exe
```

---

## 3. Determinism Contract

ShapeEngine relies on **cross-platform bit-level simulation determinism** for netcode rollback, replays, and artificial life simulation stability.

When writing simulation logic under `Engine/Simulation/` or `Engine/ECS2/`, you **MUST** follow these rules:

| Forbidden Pattern | Required Replacement | Rationale |
|---|---|---|
| `std::sin`, `std::sqrt`, `<cmath>` | `Shape::Math::Sin`, `Shape::Math::Sqrt` | Cross-platform floating-point determinism |
| `std::rand()`, `rand()` | `Shape::Simulation::DeterministicRng` | Deterministic random seed iteration |
| `std::unordered_map` in sim tick | `std::map` or `Shape::Containers::HashMap` | Deterministic iteration order |
| `std::sort` on equal keys | `std::stable_sort` | Deterministic element ordering across compilers |
| Wall-clock time (`std::chrono`) | `SimulationClock` / fixed tick counter | Frame-rate independent simulation updates |

---

## 4. Coding Standards

- **Language Standard**: C++23
- **Naming Conventions**:
  - `PascalCase` for classes, structs, enums, and filenames.
  - `camelCase` for member functions and local variables.
  - `m_` prefix for private class member variables.
  - `SHAPE_` prefix for macros.
- **Formatting**: Run `clang-format -i` before submitting code (see `.clang-format`).

---

## 5. Pull Request Workflow

1. Fork the repository and create a feature branch (`feature/your-feature`).
2. Implement your changes following the [Determinism Contract](#3-determinism-contract).
3. Ensure all tests pass locally: `cmake --build build && ./build/bin/ShapeEngineTests.exe`.
4. Add Catch2 test cases for any new engine systems or math algorithms under `EngineTests/`.
5. Open a Pull Request with a clear summary of your changes and test results.
