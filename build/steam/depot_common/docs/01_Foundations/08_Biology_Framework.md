# 08_Biology_Framework

This document formalizes DNA representation, crossover, mutations, and metabolism lifecycles.

---

## 1. Genome DNA Model
Every living shape is defined by a 12-gene genome vector:
* **Gene 0:** Target shape side count (vertices).
* **Gene 1:** Bounding radius scale.
* **Gene 2:** Restitution bounciness.
* **Gene 3:** Speed steering force.
* **Genes 4–9:** Behavioral department affinities (Survival, Engineering, Science, Economy, Military, Exploration).
* **Gene 10:** Personal mutation rate.
* **Gene 11:** Maximum age/lifespan.
* **C++ Symbol:** [GeneticsComponent](file:///C:/Users/ASUS/Documents/Primodilas/Engine/ECS/GeneticsComponent.hpp).

---

## 2. Recombination & Mutations
* **Crossover:** Sexual replication combines parent genomes using uniform random selection:
  $$\text{Child}_i = P(\text{ParentA}_i) < 0.5 \ ? \ \text{ParentA}_i : \text{ParentB}_i$$
* **Mutation:** Mutating DNA applies normal-distributed drifts to individual gene scales:
  $$\text{Gene}_i \leftarrow \text{Gene}_i + \mathcal{N}(0.0, 0.1) \quad (\text{if } \mathcal{U}(0,1) < \text{MutationRate})$$
  This mutation procedurally alters the child's geometry (adding or removing vertices).

---

## 3. Metabolism & Lifecycles
* **Energy Consumption:** Movement integration costs energy based on velocity:
  $$\Delta E = -C_{\text{metabolism}} \times |v| \times \Delta t$$
* **Starvation:** If energy reaches 0.0f, health decays by a constant rate per second.
* **Conservation of Mass:** Deceased entities decay into food resource nodes containing their remaining mass.
* **C++ Symbols:** [LifecycleComponent](file:///C:/Users/ASUS/Documents/Primodilas/Engine/ECS/LifecycleComponent.hpp) and [BiologySystem](file:///C:/Users/ASUS/Documents/Primodilas/Engine/Biology/BiologySystem.hpp).
