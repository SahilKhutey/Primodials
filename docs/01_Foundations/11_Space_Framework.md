# 11_Space_Framework

This document defines the topology of the 2D simulation plane, coordinate systems, region partitioning, and influence boundary calculations.

---

## 1. Infinite 2D Plane
The simulation universe is an unbounded continuous 2D Euclidean plane with no hard borders. Coordinates are stored as 32-bit floating-point pairs $(x, y)$ in world-space units.

* **World Units:** One world unit corresponds to a configurable number of screen pixels (default: 1.0 unit = 10 pixels at base zoom).
* **Floating-Point Precision:** To prevent precision errors at extreme coordinates (e.g. > 100,000 units from origin), entities that stray far from the active cluster may trigger an origin-shift transformation that translates all entities back toward origin while preserving relative offsets.

---

## 2. Coordinate Hierarchy
```
World Space (infinite)
     │
     ├── Region Space (chunked grid)
     │       └── Cell Size: S × S units
     │
     └── Screen Space (viewport, pixels)
             └── Camera transform applied
```
* **Region Keys:** Regions are uniquely identified by integer grid index pairs:
  $$(\text{RegionX}, \text{RegionY}) = \left( \lfloor x / S \rfloor, \lfloor y / S \rfloor \right)$$
* **Active Region Set:** Only regions containing active entities are maintained in memory; distant regions are suspended and serialized to disk.

---

## 3. Territorial Boundaries & Influence Maps
* **Civilization Territory:** Each colony's territory is defined using influence maps—grid overlays where each cell stores the dominant ownership value.
* **Voronoi Partition:** Territory boundaries are computed via Voronoi diagrams centered on Sentinel positions:
  $$V(p_i) = \{ x \in \mathbb{R}^2 \mid \|x - p_i\| \le \|x - p_j\| \ \forall j \neq i \}$$
* **Contestation:** A cell where two overlapping influence strengths are within a threshold $\varepsilon$ is marked as a **Contested Zone**, triggering diplomatic or military resolution protocols.

---

## 4. Distance Fields
* **Signed Distance Fields (SDF):** Used for rapid overlap detection and influence propagation. The signed distance from a point $\mathbf{q}$ to the nearest boundary of a convex polygon is computed analytically using half-plane tests.
* **Gradient Fields:** Gradient directions of territory influence maps guide explorer shapes toward unclaimed or resource-rich regions.
