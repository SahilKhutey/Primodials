# Testing

This document specifies unit testing, Catch2 integration, and automated fuzz validation.

---

## 1. Testing Levels

* **Unit Testing:** Verifies isolated modules (e.g. math functions, vector rotations, LCG pseudorandom outputs).
* **Integration Testing:** Verifies ECS system interactions (e.g. physics solver updating position components, collision impulse adjustments).
* **Fuzz Testing:** Runs randomized scenarios at maximum speed to detect crash conditions or memory leaks.
* **Invariance Testing:** Asserts that core physical laws (energy conservation, mass preservation) hold over long runs.

---

## 2. Catch2 Test Runner
All tests are compiled under the `ShapeEngineTests` executable target linking the Catch2 framework:
* Tests assert deterministic behavior using fixed seeds.
* Testing code is kept in the `EngineTests/` folder, mapped directly to its corresponding engine modules.
