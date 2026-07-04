<div align="center">
  <img src="https://raw.githubusercontent.com/SahilKhutey/Primodials/main/docs/assets/logo.png" alt="Primodials Logo" width="200" onerror="this.style.display='none'"/>

  # Primodials (ShapeEngine)

  **A High-Performance, Data-Oriented C++23 Simulation Engine**

  [![License: Proprietary](https://img.shields.io/badge/License-Proprietary-red.svg)](LICENSE)
  [![C++23](https://img.shields.io/badge/C%2B%2B-23-blue.svg)](https://isocpp.org/)
  [![CMake](https://img.shields.io/badge/CMake-3.20+-green.svg)](https://cmake.org/)
  [![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)]()
  [![Tests](https://img.shields.io/badge/tests-88%2F88%20passed-brightgreen.svg)]()

</div>

---

## 📖 Overview

**Primodials** is a headless, deeply optimized ecosystem and civilization simulation framework built on a custom Entity-Component-System (ECS). Designed for extreme performance and determinism, it powers complex systemic interactions without runtime heap allocations, utilizing contiguous sparse sets and bump-allocated memory arenas.

From narrowphase SAT collisions to Lockstep rollback networking, Primodials mathematical engine is designed to run massively complex scenarios in real-time.

---

## ✨ Core Features

* 🚀 **Data-Oriented ECS**: Custom contiguous sparse sets and zero-allocation memory pools for extreme cache-coherency.
* ⚛️ **Deterministic Physics**: Euler-Cromer integration, Narrowphase SAT collision solvers, and Broadphase Spatial Hash Grids.
* 🧬 **Biology & Evolution**: Lotka-Volterra dynamics, genetic crossovers, metabolic aging, and phenotypic drift.
* 🧠 **AI & Cognition**: Utility AI architectures, perception fields, short/long term memory decay, and steering behaviors.
* 🏛️ **Civilization & Economy**: Storage depot load balancing, logistics trade routes, and technology tree propagation.
* 🌐 **Lockstep Networking**: Deterministic CRC32 state verification hashing, lockstep frame stalls, and rollback synchronization.
* 🖥️ **Decoupled Architecture**: Strictly headless mechanics enabling independent visualization layers (e.g., SDL3/Vulkan) and dedicated server environments.

---

## 📂 Repository Structure

```text
Primodials/
├── Engine/             # Core simulation library (ECS, Physics, AI, Network, Math)
├── EngineTests/        # Catch2 unit test suite asserting 100% engine determinism
├── Examples/           # Reference implementations (Rendering interpolation loops)
├── docs/               # The "Simulation Bible" - 21+ Architectural Blueprints
├── CMakeLists.txt      # Root build configuration
└── LICENSE             # MIT License
```

---

## 🛠️ Build Instructions

Primodials utilizes **CMake** and requires a **C++23** compliant compiler (e.g., LLVM-MinGW, Clang, or MSVC). The engine enforces strict `-Werror` compliance to guarantee zero memory leaks or implicit cast bugs.

### 1. Clone the Repository
```bash
git clone https://github.com/SahilKhutey/Primodials.git
cd Primodials
```

### 2. Configure & Build
```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

### 3. Run Validation Tests
The engine includes a rigorous Catch2 testing suite covering math, physics, memory boundaries, and networking.
```bash
ctest -C Release --output-on-failure
```

---

## 📚 Documentation

The architecture is extensively documented. Please refer to the **[Simulation Bible](docs/README.md)** for exhaustive details on subsystem specifications, thermodynamic math formulas, and the ECS design philosophy.

* **[Engine Architecture Blueprint](docs/04_Engine/00_Engine_Architecture_Blueprint.md)**
* **[Physics & SAT Framework](docs/06_Design/Physics.md)**
* **[Lockstep Networking](docs/06_Design/Networking.md)**

---

## 📜 License

This project is licensed under a **Proprietary & Confidential License**. Complete ownership and control of the Engine is retained by the author (Sahil Khutey). 

You are strictly prohibited from using, copying, modifying, merging, publishing, distributing, sublicensing, and/or selling copies of this software without explicitly obtaining prior written permission. Any commercial use or integration into a service requires explicit authorization.

See the [LICENSE](LICENSE) file for the full text.
