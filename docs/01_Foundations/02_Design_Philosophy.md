# 02_Design_Philosophy

This document establishes the architectural and gameplay design principles guiding all engine implementations.

---

## 1. Core Principles

### Principle 1: Emergence over Scripting
* **Rule:** Never write bespoke behavior scripts for specific species or scenarios.
* **Alternative:** Define universal rules (kinematics, field emissions, utility calculations) and adjust traits (size, speed, bounciness, preferences). Complex social structures must arise organically from individual decisions.

### Principle 2: Systems over Assets
* **Rule:** Prioritize system logic over visual or audio asset variety.
* **Rationale:** The beauty of the universe lies in its behavioral dynamics. A triangle colliding and transferring kinetic energy to a square that then updates its memory logs is more valuable than complex sprite animations.

### Principle 3: Data-Oriented Design (DOD)
* **Rule:** Do not construct deeply nested object-oriented hierarchies (e.g. `virtual class Unit`).
* **Alternative:** Component data is stored contiguously in memory (`std::vector<RigidBodyComponent>`), while systems process them in linear loops. This ensures maximum cache hit rates and supports simulating millions of elements.

### Principle 4: Absolute Observability
* **Rule:** Every variable state, decision utility score, and field value must be transparent.
* **Alternative:** Develop validation frameworks, debug logging, and dev inspect hooks so every entity transaction can be analyzed and verified.

---

## 2. Gameplay Design Axioms

### Decentralized Control
The player does not issue direct click commands like in standard RTS games. The player influences the sandbox by modifying environmental parameters (spawning resources, placing threat grids, adjusting Sentinel coordination types).

### Evolution as the Ultimate Optimizer
Specialization, career shifts, and geometric morphs are selected naturally. Shapes evolve traits because they increase their efficiency ($\text{Fitness}/\text{Cost}$), matching biological ecosystem laws.
