# 11_Scenario_Ice_Age_Survival

This document specifies the setup parameters, temperature decay curves, and adaptive criteria for the Ice Age Survival scenario.

---

## 1. Setup Parameters
* **Name:** Ice Age Survival
* **Objective:** Guide a colony of shapes to survive a sustained global cooling cycle.
* **Biome:** Grassland transitions to Tundra at tick 2,000.

---

## 2. Global Cooling Curve
At tick 1,000, global baseline temperature $T_{\text{global}}$ decays exponentially:

$$T_{\text{global}}(t) = T_{\text{base}} \times e^{-\lambda (t - 1000)}$$

* **Decay Rate ($\lambda$):** Designed to freeze water nodes and reduce fertility values to $0.1$ by tick 5,000.
* **Metabolic Impact:** Metabolic cost constants increase by $2.5\times$ due to thermal cooling.

---

## 3. Survival Phenotype Criteria
To survive the winter:
* Genomes must adapt: shapes with shorter lifespans starve quickly, selecting for shapes with high lifespan genomes ($\text{Gene}_{11}$).
* Sentinels must enable "Harvest" and "Fortify" policies to store food reserves before temperature drops conclude.
* Builders must construct insulated workshops, keeping internal colony temperatures stable.
