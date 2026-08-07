# 02_Module_Architecture

This document specifies module compilation boundaries, library definitions, and include coordinates.

---

## 1. Engine Library Target
The project is split into two primary CMake compilation targets:
* **ShapeEngine (Static Library):** Compiles all core modules (Math, Memory, ECS, Physics, AI, Biology, Sentinel, Influence). Contains no windowing or drawing dependencies.
* **ShapeEngineTests (Executable):** Catch2 test runner linking the static library to run the verification suite.

---

## 2. Include Policies
To prevent circular header references, modules follow strict header structures:
* Public APIs are exposed in headers located in module subfolders (e.g. `Engine/ECS/World.hpp`).
* Headers must only include lower-layer headers. Cross-module headers must be avoided or resolved using forward class declarations:
  ```cpp
  // Forward declarations instead of inclusion
  namespace Shape {
      class World;
      struct Entity;
  }
  ```

---

## 3. CMake Dependencies
Target configurations are maintained in [CMakeLists.txt](file:///C:/Users/ASUS/Documents/Primodilas/Engine/CMakeLists.txt):
```cmake
add_library(ShapeEngine STATIC
    # Module source file paths list
)
target_include_directories(ShapeEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
```
This enables clean absolute includes relative to the project root.
