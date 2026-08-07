# 11_Physics_System

This document specifies the software requirements (SRS) for the Physics System module.

---

## 1. Subsystem Specification

* **Identity:** `shape::physics`
* **Purpose:** Euler-Cromer kinematics integration, polygon SAT contact sweeps, elastic impulse contacts solving.
* **Objectives:** Deterministic collision reactions, fast contact calculations.
* **Requirements:** Fixed updates timings, structural overlaps correction trees.
* **Responsibilities:** Summing forces vectors, broadphase hash cell checks, narrowphase normal calculations, impulse applications.
* **Subsystems:** KinematicsSolver, CollisionDetector, ConstraintSolver.
* **Managers:** `PhysicsManager`.
* **Components:** `RigidBodyComponent`, `GeometryComponent`.
* **Interfaces:** `IPhysicsWorld`, `IPhysicsSolver`.
* **Events:** `CollisionEvent` (emitted on collision impulse resolutions).
* **Resources:** Attraction acceleration profiles, frictional damping values.
* **Algorithms:** Euler-Cromer integration, Separating Axis Theorem (SAT), Baumgarte stabilization positional corrections.
* **Data Structures:** Collision manifold parameters.
* **Execution:** Updates during Phase 2 (Physics update phase).
* **Dependencies:** `shape::core`, `shape::math`, `shape::container`
* **Configuration:** Gravity attraction vectors.
* **Serialization:** Saves RigidBody positions, angles, forces parameters.
* **Debugging:** Toggle visual body boundaries and normal indicators in the editor views.
* **Testing:** Catch2 unit tests verify bounce angles matches elastic invariants.
* **Benchmarks:** SAT intersection loop timing tests.
* **Performance Targets:** Under 5ms solver updates timings.
* **Future Extensions:** Continuous Collision Detection (CCD) updates.

---

## 2. Verification & Validation (V&V)

* **Verification:** Unit tests assert that tree structures correct overlaps, maintaining shape restitution parameters over long runs.
* **Validation:** Verification that the physics engine conserves total kinetic energy during elastic collisions in isolated systems within rounding thresholds.
