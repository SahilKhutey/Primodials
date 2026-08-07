# Framework

This document specifies the execution layers, public API contracts, and interface guidelines.

---

## 1. System Integration Layers
Modules are registered under discrete framework layers:

| Layer | Primary Framework | System Contracts |
|-------|-------------------|------------------|
| **L4** | Client / Game | Bootstraps window; handles HUD input |
| **L3** | Simulation Kernel | Orchestrates clock ticks, updates world registry |
| **L2** | Systems Framework | Runs physics, AI, biology, careers logic |
| **L1** | Storage & ECS | SparseSet components, CommandBuffer deferrals |
| **L0** | Memory & Math | Allocates blocks, Vector geometry, area calculation |

---

## 2. API Contract Rules
* **No Raw Pointers:** Methods must accept/return references or entity handles.
* **Trivial Construction:** Component data structures must be trivially copyable, supporting binary serialization without custom parsers.
* **Const Safety:** Methods that do not modify state must be marked `const` to allow safe parallel read-only queries.
