# 26_Civilization_System

This document specifies colony infrastructures, building layouts, and multi-colony alliances.

---

## 1. Colony Infrastructure Types
Civilizations survive by constructing buildings using refined resources:
* **Storage Depot:** Aggregates food, minerals, and tools.
* **Workshop:** Refines raw minerals and conducts research experiments.
* **Defense Tower:** Projects a localized threat field emitter targeting hostiles.

---

## 2. Shared Storage Networks
A civilization is coordinates-connected:
* Depots are linked by carrier paths (see [10_Logistics_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/02_Simulation/10_Logistics_System.md)).
* Sentinels query connected depots to detect imbalances:
  $$\Delta \text{Stock} = \text{Stock}_A - \text{Stock}_B$$
  If $\Delta \text{Stock}$ exceeds a threshold, a transport task is dispatched to carry resources from depot A to depot B.

---

## 3. Faction Alliances
Different colonies join alliances to coordinate defense and trade. Alliances sync their global technology databases, accelerating technology unlocks across the entire civilization.
