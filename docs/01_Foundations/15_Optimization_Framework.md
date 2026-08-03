# 15_Optimization_Framework

This document defines the universal optimization principle that drives every decision, behavioral selection, evolutionary pressure, and civilization policy in the simulation.

---

## 1. The Universal Optimization Law
Every entity, from a single-cell triangle to a multi-Sentinel civilization, operates under a single governing objective:

> **Maximize Fitness while minimizing Cost.**

$$\text{OptimizationScore} = \frac{\text{Fitness}}{\text{Cost}}$$

This ratio is the fundamental selection pressure. Entities with higher scores persist and replicate; those with lower scores decay, starve, or are outcompeted.

---

## 2. Fitness Components
$$\text{Fitness} = f(E_m, R, K, N, T, S)$$

| Variable | Meaning                      |
|----------|------------------------------|
| $E_m$    | Metabolic energy level       |
| $R$      | Resources accumulated        |
| $K$      | Knowledge / information      |
| $N$      | Population count             |
| $T$      | Technology level             |
| $S$      | Structural stability         |

Fitness increases when entities successfully gather resources, survive threats, reproduce, and contribute to civilization growth.

---

## 3. Cost Components
$$\text{Cost} = g(\tau, d, E_{\text{spent}}, \rho_{\text{risk}}, \Delta S)$$

| Variable         | Meaning                                        |
|-----------------|------------------------------------------------|
| $\tau$           | Time elapsed performing the action             |
| $d$              | Distance traveled                              |
| $E_{\text{spent}}$ | Metabolic energy consumed                  |
| $\rho_{\text{risk}}$ | Probability of encountering threats      |
| $\Delta S$       | Structural damage received                     |

---

## 4. Multi-Level Optimization
The optimization principle applies at every layer of the simulation simultaneously:

| Level              | Optimizer          | Fitness Signal                        |
|-------------------|--------------------|---------------------------------------|
| Individual Shape   | Utility AI         | Energy, safety, reproduction success  |
| Career / Role      | Skill growth       | Task proficiency, rank promotion      |
| Department         | Workforce balancing | Throughput, resource deficit signals  |
| Civilization       | Sentinel policies  | Net energy surplus, territory size    |
| Species            | Evolution          | Generational survival rate            |

---

## 5. Utility Evaluation (Per-Action Scoring)
Each candidate action generates a utility score during the decision cycle:

```
UtilityScore(action) = Σ [Weight_i × Factor_i]
```

The action with the highest utility score is selected for execution. Weights and factors are modulated by emotional states, skill levels, and genetic affinities.

**Example — Gather Utility:**
$$U_{\text{Gather}} = \text{Hunger} \times 10.0 \times \frac{1.0}{1.0 + 0.02 d^2} \times G_{\text{survival}} \times S_{\text{survival}}$$

where $G_{\text{survival}}$ is the survival gene and $S_{\text{survival}}$ is the survival skill.
