# 24_Research_System

This document specifies the compilation of research points, experimental resource consumption, and discovery scans.

---

## 1. Science Points Generation
Scholars (Hexagons) generate **Science Points (SP)** through two actions:
* **Exploration Scan:** When a scholar enters previously unvisited spatial hash cells, it registers observation coordinates, generating:
  $$\Delta \text{SP} = \text{Wonder} \times 0.5$$
* **Experiment Cycle:** Working at workshops consumes raw minerals and energy, converting them to science points:
  $$5 \text{ Minerals} + 2 \text{ Energy} \rightarrow 1 \text{ SP} \times S_{\text{science}}$$

---

## 2. Research Stations (Workshops)
Workshops coordinate science shapes. They serve as inventory nodes holding raw inputs and cataloging active experiments.

---

## 3. Technology Point Transmission
Once accumulated at a workshop, SP values are transferred to the local Sentinel to be spent on technology unlocks, converting raw observations into civilization-wide upgrades.
