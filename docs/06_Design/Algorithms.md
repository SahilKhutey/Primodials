# Algorithms

This document specifies spatial hashing broadphase partitioning, SAT narrowphase tests, pathfinding, and flocking formulas.

---

## 1. Spatial Hash Grid Key Calculation
To partition entities on the infinite 2D plane:

$$\text{Key} = (\lfloor x / S \rfloor \ll 32) \mid (\lfloor y / S \rfloor \ \& \ \text{0xFFFFFFFF})$$

* **Cell Size ($S$):** Defaults to $50.0$ world units.
* Queries are $O(1)$ amortized, preventing $O(N^2)$ checks during broadphase.

---

## 2. Separating Axis Theorem (SAT)
1. Project polygon vertices onto all edge normals.
2. If projection spans overlap on all axes, a collision is registered:
   $$\text{overlap} = \min(\text{max\_A}, \text{max\_B}) - \max(\text{min\_A}, \text{min\_B})$$

---

## 3. Pathfinding & Flocking
* **Hierarchical A\*:** Computes paths across Sentinel region nodes first, then resolves local movements.
* **Cohesive Flocking:** Squad steering forces combine separation, cohesion, and alignment:
  $$\mathbf{F}_{\text{flock}} = w_1 \mathbf{F}_{\text{cohesion}} + w_2 \mathbf{F}_{\text{alignment}} + w_3 \mathbf{F}_{\text{separation}}$$
