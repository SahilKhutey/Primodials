# 27_Sentinel_System

This document specifies the Sentinel nerve center system, command coordinates synchronizations, and influence auric fields.

---

## 1. Sentinel Properties
Sentinels are specialized static or low-speed entities with a [SentinelComponent](file:///C:/Users/ASUS/Documents/Primodilas/Engine/ECS/SentinelComponent.hpp):
* **Command Radius ($R_{\text{command}}$):** Defines the boundary of the Sentinel's influence field.
* **Tech Database:** Stores unlocked blueprints and research stats.
* **Coordinate Maps:** Bounded logs of discovered resources, threats, and friendly coordinates.

---

## 2. Emitting Field Modifiers
The [SentinelSystem](file:///C:/Users/ASUS/Documents/Primodilas/Engine/Sentinel/SentinelSystem.cpp) updates Sentinel components each tick:
* Projects a `Leadership` influence field within $R_{\text{command}}$ range, boosting trust values of nearby shapes.
* Syncs memory entries from shapes inside the radius into the Sentinel's central coordinate table.
* Re-broadcasts these coordinates, syncing them into the memory logs of all shapes entering the auric boundary.

---

## 3. Sentinel Network Links
Multiple Sentinels establish virtual link graphs if their command fields overlap, allowing trade data, tech flags, and threat warnings to propagate across regions.
