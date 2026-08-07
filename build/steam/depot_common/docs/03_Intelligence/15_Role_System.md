# 15_Role_System

This document specifies the role assignment system and dynamic department affinities based on shape geometry.

---

## 1. Shape Vertex Affinities
An entity's geometry side count determines its baseline career interest weights:

| Shape (Sides) | Default Career | Primary Interest Boost                         |
|---------------|----------------|------------------------------------------------|
| Triangle (3)  | Soldier        | `interestMilitary` boosted to 2.5f             |
| Square (4)    | Builder        | `interestEngineering` boosted to 2.5f          |
| Pentagon (5)  | Merchant       | `interestEconomy` boosted to 2.5f              |
| Hexagon (6+)  | Researcher     | `interestScience` boosted to 2.5f              |

These interests increase the probability that corresponding action utilities are selected.

---

## 2. Career Progression
Entities track skill progression under three ranks:
* **Rank 1: Citizen:** Standard actions, base yields, standard speed.
* **Rank 2: Specialist:** Boosted action speeds (+25%), reduced energy costs.
* **Rank 3: Expert:** Max yield modifiers (+50%), teaching capability enabled.

---

## 3. Career Shifts
If a critical shortage occurs (e.g. food starvation sweeps the colony), Sentinels broadcast a policy overriding default shape affinities. This shifts gather utility weights, forcing soldiers and builders to work as gatherers until the crisis is resolved.
