# 03_User_Interface_Architecture

This document specifies the UI layout, decoupling rules, and screen state machines.

---

## 1. UI Decoupling Architecture
To preserve determinism and memory safety:
* The UI layout runs in a separate module and cannot write directly to component stores.
* User click selections are converted to input packets and pushed to the engine's `EventQueue`, ensuring modifications are processed during the write phase (Phase 3).
* HUD structures read simulation states via read-only queries, preventing race conditions.

---

## 2. HUD State Machine
The client manages three core screen modes:

```
[Main Menu State] ──→ [Scenario Select State] ──→ [Simulation Run State]
                                                         │
                                                         ▼
                                               [Diagnostics View State]
```

* **Main Menu:** Coordinates client settings and build configurations.
* **Simulation Run:** The active sandbox HUD (displaying entities, influence fields, and control panels).
* **Diagnostics View:** Renders performance graphs and memory allocation overlays.

---

## 3. UI Panels Layout
* **Control Bar (Top):** Speed multipliers, pause/rewind triggers, seed values, active season.
* **Selection Panel (Left):** Spawner buttons (resource magnets, threat emitters, Sentinel towers).
* **Detail Inspector (Right):** displays variables of selected shapes (needs, emotions, memory logs).
