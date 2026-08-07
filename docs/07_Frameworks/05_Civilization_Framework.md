# 05_Civilization_Framework

This document specifies the Civilization Framework.

---

## 1. Core Specifications

* **Purpose:** Colony organization, shared resource network routing, and technology tree progressions.
* **Responsibilities:** Balancing storage inventories, compiling research points, unlocking technologies, and managing Sentinel networks.
* **Interfaces:** `IColonyCoordinator`, `ITechTree`.
* **Managers:** `CivilizationManager`.
* **Systems:** `SentinelSystem` (morale fields emission), `ResearchSystem` (tech points tracking).
* **Components:** `SentinelComponent`, `InfluenceComponent`.
* **Events:** `TechnologyUnlockedEvent` (published on research completions).
* **Resources:** Science points, technology database lists.
* **Algorithms:** Delaunay triangulation logistics route graphs.
* **Threading:** Thread-safe updates run on the main thread during Phase 2.
* **Serialization:** Saves unlocked tech flags and colony inventories.
* **Testing:** Verifies that Sentinel command networks merge when boundaries overlap.
* **Debugging:** Renders colony Voronoi borders and route graph lines in the editor overlays.
