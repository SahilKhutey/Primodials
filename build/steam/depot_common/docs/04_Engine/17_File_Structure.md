# 17_File_Structure

This document outlines the file layout and directory hierarchy of the project.

---

## 1. Project Directory Layout

```
Primodilas (Root)
 ├── build/                 — CMake compilation output directory
 ├── docs/                  — Four-volume Simulation Bible
 │    ├── 01_Foundations/   — Universal axioms, math, and physics
 │    ├── 02_Simulation/    — Physics, biology, and climate systems
 │    ├── 03_Intelligence/  — AI systems, careers, and civilizations
 │    └── 04_Engine/        — Code architecture, save/load, and roadmap
 ├── Engine/                — ShapeEngine source code static library
 │    ├── AI/               — Decision and Optimization systems
 │    ├── Biology/          — Genetics and lifecycle systems
 │    ├── Core/             — Logger and basic utility templates
 │    ├── ECS/              — Sparse sets and registries
 │    ├── Math/             — Polygon calculations and matrix math
 │    └── Physics/          — Kinematics integration and SAT
 ├── EngineTests/           — Catch2 test code files
 └── Examples/              — Sandbox client application entry points
```

---

## 2. Source File Path Mappings
* **Static Lib:** Compiled files are mapped under `Engine/` (e.g. `Engine/ECS/World.cpp`).
* **Test Runner:** Verification files are mapped under `EngineTests/` (e.g. `EngineTests/AI/CareerTests.cpp`).
* **Client App:** Executables are mapped under `Examples/` (e.g. `Examples/BasicWindow.cpp`).
