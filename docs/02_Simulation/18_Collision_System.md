# 18_Collision_System

This document specifies the complete collision pipeline — broadphase spatial partitioning, narrowphase SAT detection, impulse resolution, and positional correction.

---

## 1. Collision Pipeline Overview

```
Tick N Physics Update
  │
  ├── 1. BROADPHASE
  │     └── Query SpatialHashGrid for candidate pairs within proximity
  │
  ├── 2. NARROWPHASE (SAT)
  │     └── For each candidate pair:
  │           ├── Project vertices along all edge normals
  │           ├── Check for separating axis
  │           └── Compute penetration depth and collision normal
  │
  ├── 3. IMPULSE RESOLUTION
  │     └── Apply velocity impulses to both entities
  │
  └── 4. POSITIONAL CORRECTION
        └── Separate overlapping bodies by adjusted penetration offset
```

---

## 2. Broadphase: Spatial Hash Grid

Every entity is registered in grid cells based on its AABB (axis-aligned bounding box):

```
For entity E with center (cx, cy) and radius R:
    cells = all grid cells overlapping AABB [cx-R, cx+R] × [cy-R, cy+R]
    for each cell: grid.Insert(E, cell)
```

Candidate collision pairs are entity pairs found co-existing in the same cell. Duplicate pairs are deduplicated before narrowphase.

**C++ Symbol:** [SpatialHashGrid.hpp](file:///C:/Users/ASUS/Documents/Primodilas/Engine/Physics/SpatialHashGrid.hpp)

---

## 3. Narrowphase: SAT

For each candidate pair $(A, B)$:

**Step 1 — Collect Test Axes:**
- All face normals of polygon A's edges.
- All face normals of polygon B's edges.

**Step 2 — Project and Test:**
```
for each axis n̂:
    [minA, maxA] = project(A.vertices, n̂)
    [minB, maxB] = project(B.vertices, n̂)
    if maxA < minB or maxB < minA:
        → No collision on this axis → skip pair
```

**Step 3 — Minimum Penetration Axis:**
If no separating axis is found, the axis with the minimum overlap depth is the **collision normal** $\hat{n}$ and $d$ is the **penetration depth**.

---

## 4. Impulse Resolution

```
v_rel = (v_B + ω_B × r_B) − (v_A + ω_A × r_A)
v_n   = dot(v_rel, n̂)

if v_n > 0: skip (bodies separating)

e = min(A.restitution, B.restitution)
j = -(1 + e) × v_n / (invM_A + invM_B + cross(r_A, n̂)²×invI_A + cross(r_B, n̂)²×invI_B)

v_A -= (j × invM_A) × n̂
v_B += (j × invM_B) × n̂
ω_A -= j × cross(r_A, n̂) × invI_A
ω_B += j × cross(r_B, n̂) × invI_B
```

---

## 5. Positional Correction (Baumgarte Stabilization)

To prevent sinking (numerical drift accumulation), penetrating bodies are separated each tick:

$$\text{correction} = \frac{\max(d - \text{slop}, 0.0)}{\text{invM}_A + \text{invM}_B} \times k_{\text{baumgarte}} \times \hat{n}$$

$$\mathbf{x}_A \leftarrow \mathbf{x}_A - \text{invM}_A \times \text{correction}$$
$$\mathbf{x}_B \leftarrow \mathbf{x}_B + \text{invM}_B \times \text{correction}$$

**Slop** = 0.01 (small allowed penetration to prevent jittering).
**$k_{\text{baumgarte}}$** = 0.2 (fraction of correction applied per tick).

---

## 6. Collision Damage & Events

On collision resolution, if the relative velocity magnitude exceeds a damage threshold:
$$\Delta H = \max(|v_{\text{rel}}| - V_{\text{thresh}}, 0.0) \times D_{\text{factor}} \times M_{\text{other}}$$

A `CollisionEvent` is pushed onto the `EventQueue` with both entity handles, collision normal, and damage amounts, allowing the Biology and Cognitive systems to respond.
