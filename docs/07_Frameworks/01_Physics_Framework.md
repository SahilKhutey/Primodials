# 01_Physics_Framework

This document specifies the Physics Framework.

---

## 1. Core Specifications

* **Purpose:** Universal physical simulation of moving geometries and contacts resolution.
* **Responsibilities:** Integrating velocities/positions, broadphase grid inserts, narrowphase SAT calculations, impulse calculations, Baumgarte stabilizers.
* **Interfaces:** `IPhysicsSolver`, `ISpatialPartition`.
* **Managers:** `PhysicsManager`, `CollisionManager`.
* **Systems:** `PhysicsSystem` (kinematics integration), `CollisionSystem` (overlaps resolution).
* **Components:** `RigidBodyComponent`, `GeometryComponent`.
* **Events:** `CollisionEvent` (emitted on contact resolution).
* **Resources:** Gravity-analogue attraction vector configurations, restitution constants.
* **Algorithms:** Euler-Cromer integration, Separating Axis Theorem (SAT), Spatial Hash cell key checks.
* **Threading:** Dispatches disjoint spatial hash grid cells to worker thread pools during Phase 2.
* **Serialization:** RigidBody and Geometry components are serialized as contiguous binary buffers.
* **Testing:** Catch2 unit tests assert momentum conservation and verified bouncing angles.
* **Debugging:** Visualizes boundary boxes and collision normals in the editor.
