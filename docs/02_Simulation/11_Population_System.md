# 11_Population_System

This document specifies population dynamics — birth rates, death rates, carrying capacity, migration, and demographic structure.

---

## 1. Population as an Emergent Metric

There is no global "population counter" managed by a dedicated manager. Population is simply the count of living entities with a `LifecycleComponent`. It naturally rises and falls as reproduction and mortality events occur through individual entity decisions.

$$N(t) = |\{ e \in \text{Entities} \mid e.\text{health} > 0 \}|$$

---

## 2. Birth Rate Drivers

Reproduction events occur when an entity's `AIAction::Reproduce` is selected. This action's utility rises when:

| Driver                 | Effect on Reproduction Utility              |
|-----------------------|---------------------------------------------|
| High metabolic energy  | Strong surplus incentive to reproduce       |
| Warm season (Spring)   | Seasonal modifier boosts reproduction rate |
| Low population density | Less competition; higher survival prospect  |
| High Sentinel morale   | Safety signal encourages reproduction       |

**Per-entity birth rate approximation:**
$$b_i = k_{\text{repro}} \times G_{\text{repro}} \times \frac{E_i - E_{\text{threshold}}}{100.0}$$
where $G_{\text{repro}}$ is the reproduction gene value and $E_{\text{threshold}}$ is the minimum energy required.

---

## 3. Death Rate Drivers

Entities die when:
* Health reaches 0 (from starvation, combat damage, or environmental damage).
* Age exceeds genome-defined lifespan ($\text{Gene}_{11}$).
* Catastrophic collision damage.

**Per-entity mortality hazard function (Gompertz model):**
$$h(a) = h_0 \cdot e^{\beta_{\text{age}} \cdot a}$$

where $a$ is age in ticks and $h_0$ is the baseline hazard. Older entities face exponentially increasing mortality risk.

---

## 4. Carrying Capacity Enforcement

Environmental carrying capacity $K$ is not a fixed global constant — it dynamically adjusts with available resources:

$$K(t) = \frac{\sum_j R_j(t) \times E_j}{\bar{C}_{\text{metabolism}}}$$

where $R_j$ is the richness of each resource node and $E_j$ is its energy conversion value. When $N > K$, resource scarcity intensifies, starvation rates rise, and reproduction utility falls — naturally constraining population without scripted caps.

---

## 5. Migration Dynamics

When a local region approaches carrying capacity, explorer shapes begin detecting resource-rich signals from neighboring regions via gradient maps. This triggers a wave of migration:
1. Explorer shapes navigate toward high-fertility cells.
2. Gatherers follow explorer memory broadcasts.
3. A sub-colony forms in the new region.
4. If population pressure persists, a new Sentinel is eventually deployed, formalizing the colony.

Migration is thus a fully emergent multi-step process arising from hunger, exploration utility, and memory propagation.
