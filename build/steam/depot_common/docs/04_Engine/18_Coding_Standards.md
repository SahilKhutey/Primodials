# 18_Coding_Standards

This document specifies formatting styles, naming conventions, and header rules.

---

## 1. Naming Conventions
* **Types and Classes:** PascalCase (e.g. `RigidBodyComponent`, `World`).
* **Functions and Methods:** PascalCase (e.g. `CreateEntity`, `ExpressPhenotype`).
* **Variables and Parameters:** camelCase (e.g. `vertexCount`, `currentPos`).
* **Constants and Macros:** UPPER_CASE (e.g. `ACTION_LOCK_TIME`, `SHAPE_ASSERT`).

---

## 2. Header Inclusions Guidelines
* **Order:** Include project headers first, then system library headers, separated by blank lines:
  ```cpp
  #include "ECS/World.hpp"
  #include "Core/Logger.hpp"

  #include <vector>
  #include <cmath>
  ```
* **Guard Format:** Use standard `#pragma once` at the top of all header files.

---

## 3. Safe C++ Programming Contracts
* **Heap Allocation:** Forbidden inside simulation loop ticks (System Law 5).
* **Pointer Usage:** Prefer references over raw pointers. Raw pointers must only be used where nullability is required.
* **Casting:** Use explicit casts. `dynamic_cast` is forbidden to avoid RTTI overhead; use static casts with ECS component type lookups instead.
