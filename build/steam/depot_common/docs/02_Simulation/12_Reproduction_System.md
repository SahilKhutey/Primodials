# 12_Reproduction_System

This document specifies the reproduction mechanics — the exact sequence of events, energy costs, partner selection, and offspring spawning when a shape reproduces.

---

## 1. Reproduction Modes

The simulation supports two reproduction modes depending on genome configuration:

| Mode         | Trigger                                   | Offspring Genome          |
|-------------|-------------------------------------------|---------------------------|
| **Asexual (Fission)** | Energy > threshold; no partner required | Mutated clone of parent   |
| **Sexual (Fusion)** | Two compatible entities close proximity + both consenting | Crossover of both genomes |

**Fission Threshold:**
$$E_{\text{fission}} > E_{\text{threshold}} = 70.0 + 10.0 \times \text{Gene}_{11\text{(lifespan)}}$$

Longer-lived species require more energy reserves before splitting, encoding a life-history strategy trade-off.

---

## 2. Sexual Reproduction Partner Selection

Sexual reproduction occurs when:
1. Entity A is executing `AIAction::Reproduce`.
2. A compatible entity B is within `R_{\text{sense}}` range.
3. Entity B also has Reproduce utility above a minimum threshold.
4. Both entities have sufficient energy ($E > E_{\text{threshold}}$).

**Compatibility:** Two entities are compatible if their genome vertex counts differ by ≤ 1 (similar species constraint). This prevents radically different species from hybridizing, maintaining distinct lineages.

---

## 3. Genome Crossover & Inheritance

For sexual reproduction (see detailed mutation mechanics in [13_Genetics_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/02_Simulation/13_Genetics_System.md)):

```
For each gene i in [0, 11]:
    if SimRNG.NextFloat() < 0.5:
        Child.genome[i] = ParentA.genome[i]
    else:
        Child.genome[i] = ParentB.genome[i]
```

Followed by mutation pass per gene using the child's own `MutationRate` (Gene 10).

---

## 4. Reproduction Energy Cost

Reproduction is metabolically expensive:

| Mode     | Parent Energy Cost   | Offspring Starting Energy |
|---------|----------------------|---------------------------|
| Fission  | −50% of current $E_m$ | 50% of parent's $E_m$    |
| Fusion   | −30% from each parent | 30% + 30% = 60% combined |

This cost prevents unbounded reproduction — entities must have genuine surplus energy to reproduce without risking their own survival.

---

## 5. Offspring Placement

Offspring are placed adjacent to the parent at a random offset within `R_{\text{spawn}}`:
$$\text{Offset} = R_{\text{spawn}} \times (\cos\theta, \sin\theta), \quad \theta = \text{SimRNG.NextFloat()} \times 2\pi$$

If the placement position overlaps an existing solid entity, the offset angle is rotated incrementally until a clear position is found. If no clear position exists within 8 attempts, reproduction is deferred to the next tick.
