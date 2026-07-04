# 10_Scenario_Civilization_War

This document specifies the setup parameters, faction layouts, and victory conditions for the Civilization War scenario.

---

## 1. Setup Parameters
* **Name:** Civilization War
* **Objective:** Capture the opposing colony's Sentinel node or negotiate a peace treaty.
* **Biome:** Volcanic (scarce resources, high competition).

---

## 2. Faction Layouts
Two pre-established factions spawn at opposite coordinates:
* **Colony Red (North):** Led by Sentinel A at coordinate $(0.0, 40.0)$. Spawns 20 Triangles (Soldiers) and 10 Squares (Builders). Faction relation with Sentinel B is set to $-50.0$ (Hostile).
* **Colony Blue (South):** Led by Sentinel B at coordinate $(0.0, -40.0)$. Spawns identical starting counts.

---

## 3. Victory Conditions
The scenario monitor checks for victory triggers:
* **Sentinel Destruction:** If Sentinel A's health reaches 0, Faction Blue is declared the victor.
* **Peace Accord:** If the player drops resource magnets to reduce resource competition, Faction Trust may rise above $50.0$, triggering a peace accord victory.
