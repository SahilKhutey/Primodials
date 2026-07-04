# 11_Editor_Architecture

This document specifies the in-engine editor overlays, entity inspectors, and dev sandbox tools.

---

## 1. Dev Inspector Panels
The editor provides a non-intrusive runtime UI overlay to inspect entity state:
* **Entity Inspector:** Selecting a shape displays its active components, genetics vector, and current career rank.
* **Cognitive Inspector:** Prints active utility evaluations and goal stacks (see [06_Cognitive_Framework](file:///C:/Users/ASUS/Documents/Primodilas/docs/01_Foundations/06_Cognitive_Framework.md)).
* **Spatial Grid Overlay:** Renders spatial hash partitions, active influence maps, and pathing vectors.

---

## 2. Sandbox Editing Tools
The editor can inject modifications directly into the simulation:
* Spawning entity archetypes (e.g. placing custom resource nodes or threat fields).
* Modifying component values of active shapes (e.g. manually boosting energy levels or changing genetics genomes).
* Drawing barriers and walls to test pathfinding and collision avoidance.

---

## 3. Frame Controls
* **Simulation Speed:** Adjusts clock multipliers dynamically (0x to 10,000x).
* **Step Control:** Pauses update loops, allowing developers to advance the simulation frame-by-frame for precise debugging.
