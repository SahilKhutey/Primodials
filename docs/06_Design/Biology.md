# Biology

This document specifies the genetic expression models, crossover operations, and demographic population dynamics.

---

## 1. The 12-Gene Genome Vector
Intelligent shapes carry a heritable 12-element `f32` vector:

```
Genome = [VertexCount, RadiusScale, Restitution, SpeedForce,
          AffinitySurvival, AffinityEngineer, AffinityScience, AffinityEconomy,
          AffinityMilitary, AffinityExplore, MutationRate, Lifespan]
```

At creation, these parameters are parsed to configure components (mass, speed limits, max age).

---

## 2. Mendelian Crossover & Gaussian Mutation
* **Crossover:** Selects gene values uniformly from either parent:
  $$\text{gene}_{\text{child}} = (\text{rand}() < 0.5) ? \text{gene}_{\text{parent\_A}} : \text{gene}_{\text{parent\_B}}$$
* **Mutation:** Small Gauss deviations are added per gene based on the mutation rate:
  $$\text{gene} \leftarrow \text{gene} + \mathcal{N}(0, 0.05)$$

---

## 3. Predator-Prey Oscillations (Lotka-Volterra)
Population checks scale with ecosystem limits:
$$\frac{dx}{dt} = \alpha x - \beta xy, \quad \frac{dy}{dt} = \delta xy - \gamma y$$
These dynamics emerge naturally as gatherer shapes (prey) are consumed by military shapes (predators), driving evolution strategies.
