# Modules

This document defines the modular compile-time boundaries, include policies, and linkage rules.

---

## 1. Engine Library Split
The engine compiles into isolated target modules:
* **ShapeEngine (Static Library):** Contains the math, memory, ECS, physics, and AI systems. It maintains zero dependencies on platform-specific graphics headers, enabling headless execution.
* **ShapeEngineClient (Static Library):** Compiles SDL3 wrappers, drawing pipelines, and UI layouts.
* **ShapeEngineTests (Executable):** Catch2 test runner linking the static library.

---

## 2. Interface Insulation Rules
To prevent header files from importing transitive dependencies:
* Headers use forward class declarations wherever possible.
* Implementation files (`.cpp`) contain all actual includes.
* Public methods return references or simple handles, hiding memory structures from client callers.

---

## 3. Dependency Graph Validation
The build system enforces that `/Engine` module headers do not import `/Examples` or `/Runtime` dependencies. Any violating include path triggers compile-time errors.
