# 12_Scenario_Economic_Logistics

This document specifies the setup parameters, resource placements, and optimization goals for the Economic Logistics scenario.

---

## 1. Setup Parameters
* **Name:** Economic Logistics
* **Objective:** Optimize trade throughput between two isolated resource biomes.
* **Biome:** Split Grassland (South) and Desert (North), separated by a central ridge of barren rock.

---

## 2. Resource Placements
* **South Sector:** Spawns 80 food patches and water pools; zero mineral veins.
* **North Sector:** Spawns 40 mineral veins; zero food resources.
* **Colonies:** A northern colony led by Sentinel A (Miner) and a southern colony led by Sentinel B (Farmer).

---

## 3. Optimization Metrics
To survive, both colonies must establish a trade highway:
* Faction trust is set to Allied to enable Resource treaties.
* Success is measured by net resource throughput:
  $$\text{Throughput} = \frac{\text{Food}_{\text{north}} + \text{Minerals}_{\text{south}}}{\text{Ticks}}$$
* If throughput falls below $5.0$ units/sec, both colonies collapse due to starvation or lack of tool resources.
