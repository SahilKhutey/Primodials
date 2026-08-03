# 01_Project_Vision

## 1. Executive Summary
**Polygonal Primordials** is a deterministic, emergent optimization simulation platform. The simulation is not driven by war, expansion, or linear scripted campaigns. Instead, it operates under a single thermodynamic law:

> Every organism, colony, and civilization seeks to maximize its own efficiency (optimization) under the constraints of physics, resources, time, and competition.

The platform is designed as a reusable simulation engine first, with the visual game ("Polygonal Primordials") acting as the primary client application built on top of the deterministic core.

---

## 2. Core Vision & Loop
The core gameplay loop centers on decentralized, self-optimizing shapes evolving in an infinite 2D plane:

```
Physics & Resource Grids
       │
       ▼
Organism Needs (Metabolism)
       │
       ▼
Cognitive Utility AI Decision
       │
       ▼
Kinematic Steering Force
       │
       ▼
Ecosystem Balance & Evolution
```

* **Decentralized Decision-Making:** Shapes possess individual minds and needs (Hunger, Fatigue, Safety). They evaluate their surroundings independently to schedule behaviors.
* **Emergent Specialization:** Careers (gathering, engineering, combat) emerge organically based on shape vertex affinities (e.g. Triangles naturally excel in military tasks, Squares in construction).
* **Distributed Network Nerves:** Sentinels project morale/cooperation fields and coordinate memory databases without player micromanagement.

---

## 3. High-Level Design Objectives
1. **Determinism:** The entire simulation must be bit-identical across hardware platforms, allowing lockstep synchronization, replay snapshots, and rollback.
2. **Emergence over Scripting:** Features arise from simple systems interacting rather than hand-coded scripts.
3. **Data-Oriented ECS Design:** Code is structured to maximize CPU cache locality, separating components (data) from systems (behavior) to scale to millions of entities.
