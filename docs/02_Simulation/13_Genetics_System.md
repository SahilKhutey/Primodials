# 13_Genetics_System

This document specifies the full genome model — gene definitions, crossover operators, mutation operators, and phenotype expression.

---

## 1. The Genome Vector

Every living entity carries a 12-element `f32` genome. Values are normalized to biologically meaningful ranges during phenotype expression.

| Index | Gene Name         | Raw Range    | Phenotype Expression                         |
|-------|------------------|--------------|----------------------------------------------|
| 0     | VertexCount       | 3.0 – 9.0   | Round to nearest integer → polygon shape     |
| 1     | RadiusScale       | 0.5 – 2.0   | Bounding radius × RadiusScale                |
| 2     | Restitution       | 0.1 – 0.9   | Bounciness coefficient in collision          |
| 3     | SpeedForce        | 0.5 – 2.0   | Steering force multiplier                    |
| 4     | AffinitySurvival  | 0.0 – 1.0   | Weight for Gather utility                    |
| 5     | AffinityEngineer  | 0.0 – 1.0   | Weight for Build/Craft utility               |
| 6     | AffinityScience   | 0.0 – 1.0   | Weight for Research utility                  |
| 7     | AffinityEconomy   | 0.0 – 1.0   | Weight for Trade/Carry utility               |
| 8     | AffinityMilitary  | 0.0 – 1.0   | Weight for Combat/Defend utility             |
| 9     | AffinityExplore   | 0.0 – 1.0   | Weight for Explore utility                   |
| 10    | MutationRate      | 0.01 – 0.5  | Probability of mutation per gene per birth   |
| 11    | Lifespan          | 0.5 – 3.0   | Lifespan multiplier × base ticks             |

---

## 2. Phenotype Expression

Raw gene values are mapped to engine-usable parameters at entity creation:

```cpp
void ExpressPhenotype(GeneticsComponent& g, GeometryComponent& geom,
                      RigidBodyComponent& rb, LifecycleComponent& lc) {
    geom.vertexCount    = std::clamp((int)std::round(g.genome[0]), 3, 9);
    geom.boundingRadius = BASE_RADIUS * std::clamp(g.genome[1], 0.5f, 2.0f);
    rb.restitution      = std::clamp(g.genome[2], 0.1f, 0.9f);
    rb.mass             = ComputeArea(geom) * DENSITY;
    lc.maxAge           = BASE_LIFESPAN_TICKS * std::clamp(g.genome[11], 0.5f, 3.0f);
}
```

Phenotype is re-expressed whenever a genome is modified (mutation or directed genetic editing via tech unlock).

---

## 3. Crossover Operator (Mendelian)

Uniform crossover selects each gene independently from either parent:

```
for i in 0..11:
    r = SimRNG.NextFloat()   // [0.0, 1.0)
    child.genome[i] = (r < 0.5) ? parentA.genome[i] : parentB.genome[i]
```

Future extension: **n-point crossover** where a random split point divides genome blocks, creating more correlated trait inheritance.

---

## 4. Mutation Operator

After crossover, each gene undergoes independent mutation evaluation:

```
for i in 0..11:
    if SimRNG.NextFloat() < child.genome[MUTATION_RATE]:
        delta = SimRNG.NextGaussian(0.0, 0.05)
        child.genome[i] = clamp(child.genome[i] + delta, gene_min[i], gene_max[i])
```

**Gaussian Standard Deviation:** 0.05 (5% of normalized gene range). This produces small, incremental drift aligned with biological point-mutation models.

**Large Mutation Event:** With probability $P_{\text{macro}} = 0.001$, a macro-mutation occurs: the vertex count gene shifts by ±1, adding or removing a polygon edge. This models chromosomal structural variation.

---

## 5. Gene Linkage & Epistasis

Some genes have coupled effects:
* **VertexCount + AffinityMilitary:** Triangle shapes (3 vertices) receive a multiplicative bonus to Military affinity — reflecting the shape's sharp geometry.
* **RadiusScale + SpeedForce:** Larger entities incur a speed penalty: $v_{\text{max}} = S_{\text{force}} / M$. Large size gains defense and mass; small size gains speed.
* **MutationRate + Lifespan:** High mutation rates correlate with shorter lifespans (fast evolutionary turnover strategy vs. long-lived stability strategy).
