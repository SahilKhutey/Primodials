# 04_Intelligence_Framework

This document specifies the Intelligence Framework.

---

## 1. Core Specifications

* **Purpose:** Universal cognitive modeling, sensory perception, and Utility AI decisions.
* **Responsibilities:** Querying spatial maps, updating memory logs, calculating need drive urgencies, evaluating utility options.
* **Interfaces:** `IPerceptionSensor`, `IDecisionPlanner`.
* **Managers:** `CognitionManager`.
* **Systems:** `PerceptionSystem`, `AttentionSystem`, `DecisionSystem`.
* **Components:** `MindComponent`, `AIComponent`.
* **Events:** `TargetSelectedEvent` (emitted on new target lock).
* **Resources:** Base utility weight curves configurations.
* **Algorithms:** Exponential need curves (Homeostasis), Utility AI selection, memory decay.
* **Threading:** Tiers L1/L2 updates. Read operations are split across worker threads, while decisions serialize results.
* **Serialization:** Saves need states, emotional values, and memory ring buffers.
* **Testing:** Asserts that high fear overrides hunger drives.
* **Debugging:** Renders active goal stacks and computed utility values above shape meshes.
