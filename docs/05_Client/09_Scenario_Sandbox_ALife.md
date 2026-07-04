# 09_Scenario_Sandbox_ALife

This document specifies the setup parameters, initial populations, and target metrics for the Artificial Life Sandbox scenario.

---

## 1. Setup Parameters
* **Name:** ALife Sandbox
* **Objective:** Establish a stable biological equilibrium between trophic levels.
* **Biome:** Grassland (baseline fertility $1.0$, moderate temperature $0.5$).
* **RNG Seed:** Randomized or user-defined.

---

## 2. Initial Populations
* **Resource Nodes:** 100 food patches scattered uniformly within a $100 \times 100$ unit coordinate box.
* **Herbivores:** 40 Triangles (3 vertices) with high Survival affinities and low Speed multipliers.
* **Predators:** 10 Hexagons (6 vertices) with high Military/Aggression affinities.

---

## 3. Targeted Equilibrium Outcome
The scenario runs indefinitely to evaluate ecosystem stability:
* Monitors population oscillations (predator-prey Lotka-Volterra loops).
* Success is achieved if the Shannon Diversity Index remains above $1.0$ for over 10,000 ticks, confirming that mutations and selection pressures prevent total species extinction.
