# 13_Testing_Framework

This document specifies unit testing protocols, Catch2 integration, and deterministic test seed pipelines.

---

## 1. Test Runner Target
All unit and integration tests are compiled under the `ShapeEngineTests` executable target:
* **Framework:** Catch2 v3.4.0.
* **Location:** Source files are maintained in `EngineTests/` directory, mapped directly to their matching engine modules (e.g. `EngineTests/Math/MathTests.cpp`).

---

## 2. Mock Registries
To isolate tested subsystems:
* Tests construct localized `World` registries.
* Mock entities are populated with specific component properties (e.g. creating pre-configured RigidBody components for SAT collision tests).
* Mock components are allocated using the standard `PoolAllocator` to verify memory safety during tests.

---

## 3. Deterministic Seed Pipelines
To ensure test repeatability, all random inputs are generated using fixed-seed pseudorandom generators:
```cpp
// Set a fixed seed to guarantee identical test outcomes
SimRNG testRNG(12345);
```
Tests must assert exact numerical outcomes, ensuring zero variance between runs.
