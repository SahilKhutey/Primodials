# 13_Matter_Framework

This document defines the material properties of physical entities — density, structural integrity, phase states, and resource compositions.

---

## 1. Material Properties
Every physical entity carries a set of intrinsic material properties derived from its genome and environment:

| Property           | Symbol         | Description                                            |
|-------------------|----------------|--------------------------------------------------------|
| Mass               | $M$            | Computed from area × density: $M = A \times \rho$     |
| Density            | $\rho$         | Gene-derived; affects momentum and collision force     |
| Structural Integrity | $\sigma$     | Max force before deformation or fracture               |
| Restitution        | $e$            | Bounciness coefficient $[0.0, 1.0]$                    |
| Friction           | $\mu$          | Surface drag against contacts                          |
| Thermal Conductivity | $\kappa$     | Rate of heat exchange with surrounding cells           |

---

## 2. Mass Computation
Mass is dynamically computed from geometry:
$$M = \rho \times A$$
where $A$ is the Shoelace polygon area (see [05_Mathematical_Framework](file:///C:/Users/ASUS/Documents/Primodilas/docs/01_Foundations/05_Mathematical_Framework.md)).

Mutant offspring that grow additional vertices have correspondingly larger mass, requiring more energy to sustain and move. This creates a natural **size vs. speed trade-off** enforced by evolutionary pressure.

---

## 3. Phase States
Matter in the simulation exists in three discrete phases:

| Phase  | Description                                     | Mobility       |
|--------|-------------------------------------------------|----------------|
| Solid  | Standard shape entities (polygons)              | Full kinematic |
| Liquid | Chemical pools (water, chemical solutions)      | Grid-diffusion |
| Gas    | Environmental gasses (CO₂, steam)              | Passive spread |

Phase transitions are triggered by local temperature exceeding defined thresholds:
$$T_{\text{melt}}: \text{Solid} \rightarrow \text{Liquid}, \quad T_{\text{vaporize}}: \text{Liquid} \rightarrow \text{Gas}$$

---

## 4. Resource Node Compositions
Resource nodes on the 2D plane are not uniform — they carry elemental compositions that interact with biological and chemical systems:

| Resource Type  | Composition          | Energy Value |
|---------------|----------------------|--------------|
| Food Patch     | $C_6H_{12}O_6$      | High         |
| Mineral Vein   | Fe, SiO₂, Ca        | Medium       |
| Water Pool     | $H_2O$              | Low (hydration) |

**Conservation:** When a resource node is fully consumed, its elemental components are absorbed into the consuming entity. When the entity dies, those components return as a decayed resource node, satisfying Axiom 5 (Conservation).
