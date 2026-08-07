# 02_Perception_System

This document specifies the perception system — how entities query their environment to detect resources, threats, and allies.

---

## 1. Spatial Hash Grid Queries
Perception begins by querying the Spatial Hash Grid. The entity scans all grid cells overlapping its circular **Sensory Radius** ($R_{\text{sense}}$):

$$\mathcal{P}_{\text{candidate}} = \text{QuerySpatialGrid}(p_{\text{self}}, R_{\text{sense}})$$

This broadphase filter limits subsequent calculations to nearby candidate entities.

---

## 2. Occlusion & Field of View (FOV)
An entity can only perceive a candidate if it lies within its visual arc and is not occluded by structural walls:

```
          \    Visual Field    /
           \                  /
            \     Resource   /
             \       ●      /
              \            /
               \    Wall  /
                \   ■■■  /
                 \  x   /   ← Target Occluded
                  \    /
                   Self (Angle θ)
```

**Field of View Check:**
$$\cos(\alpha) = \frac{\mathbf{v} \cdot \mathbf{d}}{|\mathbf{v}| |\mathbf{d}|} \ge \cos\left(\frac{\text{FOV}_{\text{angle}}}{2}\right)$$
where $\mathbf{v}$ is the entity's forward heading direction and $\mathbf{d}$ is the displacement vector to the candidate.

**Occlusion Test:**
A raycast is projected from $p_{\text{self}}$ to $p_{\text{candidate}}$. If the ray intersects a structural entity (building or wall), the candidate is occluded and omitted from perception outputs.

---

## 3. Sensory Component Layout
Entities carry sensory properties mapped from their genomes:
* **Sensory Radius ($R_{\text{sense}}$):** Scales with `Gene 1` (RadiusScale) and `Gene 11` (Lifespan).
* **Field of View Arc ($\text{FOV}_{\text{angle}}$):** Defaults to $180^\circ$, expandable via technology unlocks.
* **Refresh Rate:** Determines the tick interval between perception updates (default: every 2 ticks).
