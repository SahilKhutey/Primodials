# 12_AI_System

This document specifies the software requirements (SRS) for the AI System module.

---

## 1. Subsystem Specification

* **Identity:** `shape::ai`
* **Purpose:** Sensory perception grid sweeps, memory log decays, need-based Utility AI decisions.
* **Objectives:** Dynamic careers preferences, pathing calculations, structured behaviors.
* **Requirements:** Zero allocations during pathfinding runs, hysteresis timers checks.
* **Responsibilities:** Summing local influence fields, forgetting obsolete locations, evaluating utility equations, updating goal stacks.
* **Subsystems:** PerceptionScanner, MemoryTracker, PlannerUtility.
* **Managers:** `CognitionManager`.
* **Components:** `MindComponent`, `AIComponent`.
* **Interfaces:** `IPerceptionSensor`, `IDecisionPlanner`.
* **Events:** `TargetFocusChanged` (emitted on attention switches).
* **Resources:** Base career affinities database tables.
* **Algorithms:** Exponential need curves (Homeostasis), Utility AI selection, memory decay curves.
* **Data Structures:** Memory ring buffers.
* **Execution:** Ticks updates tier mappings during Phase 2 updates.
* **Dependencies:** `shape::core`, `shape::math`, `shape::container`
* **Configuration:** Needs decay rate metrics.
* **Serialization:** Saves need states, emotional parameters, and coordinates logs.
* **Debugging:** Renders computed utility values over inspected entities.
* **Testing:** Asserts that starving shapes select Food targets over exploration coordinates.
* **Benchmarks:** Pathfinding computation speeds profiling.
* **Performance Targets:** Amortized AI tick workloads.
* **Future Extensions:** GOAP (Goal-Oriented Action Planning) pathing solvers.

---

## 2. Verification & Validation (V&V)

* **Verification:** Unit tests assert that the memory ring buffer evicts older coordinate records when storage limits are exceeded.
* **Validation:** Validation that shape behaviors match realistic ecological profiles, where herbivores gather near agricultural depots and withdraw during threat alerts.
