# 20_Glossary

This document defines all canonical terminology used across the Simulation Bible. When a term appears in any specification document, its definition here is authoritative.

---

## A

| Term | Definition |
|------|-----------|
| **Action** | A discrete behavior executed by an entity during a tick (e.g. Gather, Explore, Escape). |
| **Arena Allocator** | A contiguous memory block used for transient per-tick allocations; reset at the end of each tick. |
| **Axiom** | An immutable rule of the universe that no system may violate. |

---

## B

| Term | Definition |
|------|-----------|
| **Broadphase** | The coarse collision detection pass using the Spatial Hash Grid to eliminate non-overlapping entity pairs. |

---

## C

| Term | Definition |
|------|-----------|
| **Career** | The role specialization of an entity, defining its preferred task assignment (Gatherer, Engineer, Soldier, etc.). |
| **Carrying Capacity (K)** | The maximum population a given environment can sustainably support. |
| **CommandBuffer** | A deferred mutation queue; structural ECS changes (create/destroy entity) are submitted here and flushed post-tick. |
| **Component** | A plain data struct attached to an entity, storing a specific aspect of its state. |

---

## D

| Term | Definition |
|------|-----------|
| **Dense Array** | The contiguous data array in a Sparse Set, iterated during system updates for cache efficiency. |
| **Determinism** | The property that identical inputs always produce identical simulation outputs. |

---

## E

| Term | Definition |
|------|-----------|
| **ECS (Entity Component System)** | The architectural pattern separating entity identity, data (components), and behavior (systems). |
| **Entity** | A 32-bit handle (20-bit index + 12-bit generation) uniquely identifying a simulation object. |
| **Emergence** | Complex macroscopic behavior arising from simple local rules and interactions. |

---

## F

| Term | Definition |
|------|-----------|
| **Fitness** | A composite score measuring an entity's evolutionary success based on energy, reproduction, and resource accumulation. |
| **Fixed Timestep** | The invariant simulation tick duration of exactly $16666\mu\text{s}$ (60 Hz). |

---

## G

| Term | Definition |
|------|-----------|
| **Gene** | A single scalar value within a genome vector encoding a specific trait (speed, mutation rate, department affinity). |
| **Genome** | A 12-element `f32` vector defining all heritable traits of a living entity. |

---

## I

| Term | Definition |
|------|-----------|
| **Influence Field** | A spatial emission projected by an entity that modifies the state of nearby entities (e.g. Fear, Leadership, Knowledge). |

---

## M

| Term | Definition |
|------|-----------|
| **Memory Log** | A bounded ring buffer of past observations stored inside a `MindComponent`. |
| **Moment of Inertia (I)** | Resistance of a polygon to angular acceleration, computed from its mass distribution. |
| **Mutation Rate** | The probability per gene that a heritable trait drifts during reproduction. |

---

## N

| Term | Definition |
|------|-----------|
| **Narrowphase** | The precise collision detection pass using SAT (Separating Axis Theorem) on candidate pairs from broadphase. |

---

## O

| Term | Definition |
|------|-----------|
| **OptimizationScore** | The ratio $\text{Fitness} / \text{Cost}$; the universal metric for evolutionary selection. |

---

## P

| Term | Definition |
|------|-----------|
| **Pool Allocator** | A fixed-size block memory manager for specific component types; $O(1)$ alloc/dealloc, zero fragmentation. |

---

## R

| Term | Definition |
|------|-----------|
| **Rank** | The career promotion tier of an entity (Rank 1: Citizen, Rank 2: Specialist, Rank 3: Expert). |
| **Restitution (e)** | The bounciness coefficient controlling post-collision velocity retention, $e \in [0.0, 1.0]$. |

---

## S

| Term | Definition |
|------|-----------|
| **SAT** | Separating Axis Theorem — the narrowphase algorithm for detecting convex polygon overlaps. |
| **Sentinel** | A specialized entity that functions as a civilization's nerve center, projecting influence fields and coordinating memory. |
| **SimRNG** | The simulation's deterministic random number generator, seeded from the world seed. |
| **Sparse Set** | The ECS storage structure pairing a sparse index array with a dense data array for $O(1)$ access and $O(N)$ iteration. |

---

## T

| Term | Definition |
|------|-----------|
| **Territory** | A region of the 2D plane claimed by a civilization, computed via Voronoi partitioning of Sentinel positions. |
| **Tick** | A single fixed-duration simulation update step ($16666\mu\text{s}$). |

---

## U

| Term | Definition |
|------|-----------|
| **Utility Score** | A numeric value computed per action candidate during AI decision evaluation, representing its desirability. |

---

## V

| Term | Definition |
|------|-----------|
| **Voronoi Diagram** | A spatial partition where each cell contains all points closest to a given seed point (e.g. a Sentinel). |

---

## W

| Term | Definition |
|------|-----------|
| **World** | The central ECS registry managing all entities, component stores, and system coordination. |
| **World Seed** | A 64-bit integer used to seed the `SimRNG`; stored in every snapshot to ensure reproducibility. |
