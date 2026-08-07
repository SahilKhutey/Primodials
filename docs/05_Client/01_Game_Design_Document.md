# 01_Game_Design_Document

This document specifies the core game design loops, player agency models, risk vs reward metrics, and difficulty balancing parameters.

---

## 1. Game Design Loop
The primary loop translates engine simulations into an engaging gameplay sandbox:

```
[Simulation State Runs] ──→ [HUD Renders Visuals] ──→ [Player Influences Sandbox]
          ▲                                                     │
          └─────────── [Engine Processes Inputs] ◄──────────────┘
```

* **Observation:** The player scans the ecosystem, observing shape careers, trade routes, and military conflicts.
* **Indirect Influence:** The player does not control shapes. They influence the simulation by dropping resources, adjusting weather temperatures, or modifying Sentinel policies.

---

## 2. Player Agency Model
The player acts as an "Environmental Deity":
* **Attractors:** Placing resource magnets to guide shape migration.
* **Repellers:** Setting dangerous threat fields (e.g. fire, hazard zones) to redirect trade routes.
* **Sentinel Directive:** Altering the priorities of local Sentinels (e.g. enforcing "Fortify" or "Harvest" directives).

---

## 3. Difficulty Scaling
Difficulty is governed by climate volatility and resource density parameters:
* **Easy:** High resource spawn rates, stable seasonal temperatures, low predator aggression.
* **Expert:** Scarce mineral veins, severe winters with high metabolic costs, frequent storms.
