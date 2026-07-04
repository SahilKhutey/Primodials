# 09_Geometry_Framework

This document specifies the geometry models, regular polygon layouts, Voronoi partitioning, and mesh caching patterns.

---

## 1. Shape Representation
* **Vertices Layout:** Polygons are defined as an ordered list of 2D vertices relative to their centroid $(0, 0)$.
* **Regular Polygons Generation:** Procedural generation maps vertices along a circle radius:
  $$\begin{aligned}
  x_i &= R \sin(i \cdot \theta_{\text{step}}) \\
  y_i &= R \cos(i \cdot \theta_{\text{step}})
  \end{aligned}$$
  where $\theta_{\text{step}} = 2\pi / N$ for $N$ sides.
* **C++ Symbol:** [PolygonGeometry](file:///C:/Users/ASUS/Documents/Primodilas/Engine/Math/PolygonGeometry.hpp).

---

## 2. Spatial Partitioning Algorithms
* **Voronoi Regions:** Civilizations use Voronoi diagrams to partition resource territory. Cell boundaries are defined by the midpoints between neighboring Sentinels:
  $$V(p_i) = \{ x \in \mathbb{R}^2 \mid \|x - p_i\| \le \|x - p_j\| \ \forall \ j \neq i \}$$
* **Delaunay Triangulation:** Used by logistics systems to build trade routes between colonies. It connects nodes such that no node is inside the circumcircle of any triangle.

---

## 3. ECS Mesh Cache
* **Geometry Cache:** To prevent recalculating properties (area, centroid, bounding radius) every tick, the values are cached inside components during mesh creation.
* **C++ Symbol:** [GeometryComponent](file:///C:/Users/ASUS/Documents/Primodilas/Engine/ECS/GeometryComponent.hpp).
