# 17_Skill_System

This document specifies individual skill scaling, caps, and the skill decay model.

---

## 1. Skill Registry
Entities track skills in their [CareerComponent](file:///C:/Users/ASUS/Documents/Primodilas/Engine/ECS/CareerComponent.hpp):
* `skillSurvival` (Gathering and metabolism efficiency)
* `skillExploration` (Sensory speed and wonder thresholds)
* `skillConstruction` (Building and repairs speed)
* `skillScience` (Technology points yield and memory retention)
* `skillMilitary` (Attack damage and fear resilience)
* `skillEconomy` (Inventory limits and pricing evaluations)

---

## 2. Skill Caps & Scaling
* **Base Skill Cap:** 10.0 units.
* **Level Thresholds:**
  - Specialist (Rank 2) requires any single skill $\ge 2.5$.
  - Expert (Rank 3) requires any single skill $\ge 5.0$.

---

## 3. Skill Decay Invariant
To prevent entities from becoming master-of-all shapes, inactive skills decay over time:

$$\text{Skill}_{\text{inactive}, t+1} = \max(1.0, \text{Skill}_{\text{inactive}, t} - C_{\text{skill\_decay}} \cdot \Delta t)$$

* **Decay Rate ($C_{\text{skill\_decay}}$):** $0.01$ units/sec.
* **Active Exclusions:** The skill corresponding to the current active action does not decay.
