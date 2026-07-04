# 15_Mutation_System

This document specifies the mutation subsystem — the exact mechanisms, probability distributions, and phenotypic consequences of genetic mutations.

---

## 1. Mutation Classification

Mutations are categorized by their magnitude and scope:

| Class             | Probability           | Genome Effect                                  | Phenotype Effect                             |
|------------------|-----------------------|------------------------------------------------|----------------------------------------------|
| **Point Mutation** | `MutationRate` per gene | $\pm \mathcal{N}(0, \sigma_{\text{point}})$ | Small continuous trait adjustment             |
| **Macro Mutation** | $P_{\text{macro}} = 0.001$ per birth | VertexCount ±1    | Adds or removes one polygon edge             |
| **Silent Mutation** | Always occurs internally | Gene drift within no-effect zone           | No observable phenotype change               |
| **Lethal Mutation** | $P_{\text{lethal}} = 0.0005$ | Any critical gene → extreme value  | Immediate health = 0.0 at birth              |

---

## 2. Point Mutation Algorithm

```
σ_point = 0.05   // 5% standard deviation of normalized gene range

for each gene i in child.genome:
    if SimRNG.NextFloat() < child.genome[GENE_MUTATION_RATE]:
        delta = SimRNG.NextGaussian(mean=0.0, stddev=σ_point)
        child.genome[i] += delta
        child.genome[i] = clamp(child.genome[i], GENE_MIN[i], GENE_MAX[i])
```

---

## 3. Macro Mutation (Vertex Shift)

The vertex count mutation is the most consequential — it changes the entity's fundamental shape class:

```
if SimRNG.NextFloat() < P_macro:
    direction = (SimRNG.NextFloat() < 0.5) ? -1 : +1
    new_vertices = clamp(current_vertices + direction, 3, 9)
    child.genome[GENE_VERTEX_COUNT] = (f32)new_vertices
    // Re-express geometry immediately
    RegeneratePolygonVertices(child, new_vertices)
```

**Shape Evolution Table:**

| Mutation Event      | Result                              | Ecological Consequence                 |
|--------------------|-------------------------------------|----------------------------------------|
| 3 → 4 (Tri → Sq)   | Military → Engineering shift         | Career affinity rebalance              |
| 4 → 5 (Sq → Pent)  | Engineering → Economy shift          | Trade role emergence                   |
| 6 → 7 or 7 → 8     | Higher complexity → slower, heavier  | Tank-class entity emerges              |
| Any → 3 (reduction) | Reversion to triangle               | Fast, aggressive predator returns      |

---

## 4. Mutation Rate Evolution

The mutation rate gene itself evolves under selection. In stable, resource-rich environments, low mutation rates are selected (preserve successful genomes). In volatile, rapidly changing environments, high mutation rates are selected (accelerate adaptation):

$$\text{Optimal Rate} \approx \frac{1}{\text{Genome Stability Pressure}}$$

This is an implementation of the **evolutionary stable mutation rate hypothesis** — and it arises without any explicit guidance.

---

## 5. Mutation Tracking (Phylogenetic Tree)

Each entity stores a `lineageID` — a hash of its ancestor chain. On each reproduction:
$$\text{lineageID}_{\text{child}} = \text{CRC32}(\text{lineageID}_{\text{parent}} \| \text{tickCount})$$

This allows the debug overlay and analytics system to reconstruct **phylogenetic trees** — visualizing how species diverged from common ancestors over time.
