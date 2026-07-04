# 07_Simulation_Architecture

This document specifies the simulation loop schedule, event buses synchronization, and tick phase boundaries.

---

## 1. Tick Phases
A single simulation tick is divided into three distinct phases:

```
[Phase 1: Input & Read] ──→ [Phase 2: Physics & Update] ──→ [Phase 3: Write & Mutate]
```

1. **Phase 1: Input & Read:** Poll platform events and copy external inputs into the `EventQueue`.
2. **Phase 2: Physics & Update:** Run systems (Physics, Influence, Biology, AI, Careers) in strict sequence. Systems write mutations to the `CommandBuffer`.
3. **Phase 3: Write & Mutate:** Flush the `CommandBuffer` to apply entity creation, deletion, and component adjustments. Reset the Arena Allocator.

---

## 2. Event Bus Synchronization
Events are processed with a 1-tick delay to ensure deterministic ordering:
* Systems push events to the `EventQueue` during Phase 2.
* These events are read by systems in the subsequent tick, preventing race conditions or order-dependent behaviors.

---

## 3. CommandBuffer Flushing
All structural modifications are applied in batch mode during Phase 3:
* **Entites Deletion:** Components are removed from sparse sets first, then entity indices are recycled.
* **Entities Creation:** New IDs are generated and components are allocated.
