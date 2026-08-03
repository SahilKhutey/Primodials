# 02_Biology_Framework

This document specifies the Biology Framework.

---

## 1. Core Specifications

* **Purpose:** Coordinates dynamic lifecycles, metabolic rates, and genetic phenotypes expression.
* **Responsibilities:** Initializing infant properties, subtracting energy based on movement/cognition speeds, health decay checks.
* **Interfaces:** `IBiochemicalReactor`, `ILifecycleModel`.
* **Managers:** `EcosystemManager`, `BiologyManager`.
* **Systems:** `BiologySystem` (lifecycle and health updates), `MetabolismSystem` (energy consumption loops).
* **Components:** `GeneticsComponent`, `LifecycleComponent`.
* **Events:** `DeathEvent` (starvation or age eviction), `MaturationEvent` (infant to adult transitions).
* **Resources:** Base metabolic drain rates, max lifespan configurations.
* **Algorithms:** Gompertz aging curves, exponential metabolic scaling.
* **Threading:** Runs read-only queries in parallel across contiguous component slices.
* **Serialization:** Genetics and Lifecycle components are serialized in binary save formats.
* **Testing:** Verifies that starvation triggers health reduction when energy is zero.
* **Debugging:** Renders green-to-yellow lifecycle states on entity visual meshes.
