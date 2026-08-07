# 14_Evolution_System

This document specifies the evolutionary dynamics model — how selection pressure, inheritance, and adaptation operate across generations to produce macro-scale evolutionary change.

---

## 1. Evolution as Continuous Optimization

Evolution in this simulation is not a batch process run at discrete "generation" intervals. It is continuous — every reproduction and death event is a selection event. Fitness is evaluated implicitly through survival outcomes.

> **Selection Rule:** Entities that survive long enough to reproduce pass their genome to the next generation. Entities that die before reproducing leave no genetic descendants.

This matches **non-overlapping generation** biological models without requiring artificial generation counters.

---

## 2. Fitness Landscape

The fitness landscape is dynamic — it changes as the environment, population, and competition shift. There is no single global fitness function. Instead, fitness is contextual:

| Context               | High-Fitness Traits                              |
|----------------------|--------------------------------------------------|
| Resource-scarce env.  | High Survival affinity; high SpeedForce          |
| Dense population      | High RadiusScale (defense); high MutationRate    |
| Military conflict     | High AffinityMilitary; Triangle vertex shape     |
| Knowledge economy     | High AffinityScience; long Lifespan              |

The fitness landscape shifts as civilizations develop, resources deplete, and climates change — ensuring evolution never converges on a single dominant strategy.

---

## 3. Selection Mechanisms

| Mechanism            | Description                                                 |
|---------------------|-------------------------------------------------------------|
| **Natural Selection** | High-energy entities outcompete low-energy entities for mates and resources |
| **Sexual Selection**  | Partner choice favors high-skill-rank entities (visible through career rank signals) |
| **Kin Selection**     | Entities with shared Sentinel affiliation cooperate, increasing inclusive fitness |
| **Drift**             | In small populations, random mortality causes allele frequency shifts (genetic drift) |

---

## 4. Speciation

Speciation occurs when two subpopulations become reproductively isolated:
* **Geographic Isolation:** Colonies separated by large distances cease interbreeding; genomes diverge via independent mutation.
* **Behavioral Isolation:** Civilizations with divergent career compositions develop different social norms (future culture encoding), creating assortative mating pressure.
* **Chromosomal Barrier:** Vertex count genes that diverge by ≥ 2 between populations trigger the **compatibility** check failure in the Reproduction System, preventing hybrid offspring.

---

## 5. Macro-Evolutionary Events

| Event                | Trigger                                         | Effect                                             |
|---------------------|------------------------------------------------|----------------------------------------------------|
| **Adaptive Radiation**| Novel biome colonized; low competition         | Rapid divergence into multiple vertex-count types  |
| **Evolutionary Bottleneck** | Population crash (e.g. blizzard)       | Loss of genetic diversity; accelerated drift       |
| **Evolutionary Arms Race** | Two competing civilizations            | Mutual escalation of Military affinity and mass    |
| **Niche Partitioning** | Resource competition between two species      | Divergence into distinct resource preferences      |
