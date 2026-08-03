# 14_Communication_System

This document specifies the communication protocols, spatial threat warnings, and messaging bandwidth constraints.

---

## 1. Spatial Warnings (Alarm Calls)
When an entity detects a high-strength threat (saliency score > 80.0), it emits an **Alarm Call**:
* The calling entity spawns a temporary `InfluenceComponent` of type `Fear` at its coordinates.
* The Fear field expands rapidly up to a radius of $30.0$ units.
* All nearby entities entering this field write the threat location directly into their memory logs, triggering immediate Escape overrides.

---

## 2. Bandwidth Constraints
Entities cannot exchange unlimited memory tables instantly:
* **Contact Bandwidth:** During social syncs, shapes exchange a maximum of 3 memory entries per interaction tick.
* **Message Drops:** High local entity density congests fields, causing a 10% packet drop rate per 10 active shapes in the same grid cell.

---

## 3. Communication Network Layout
```
Threat Detected ──→ Alarm Call (Fear field) ──→ Neighbors escape
                                                   │
                                                   ▼
                                         Sentinel Syncs coordinates
                                                   │
                                                   ▼
                                        Global Threat Map updated
```
This multi-tier broadcast system allows local warnings to escalate into colony-wide defense postures.
