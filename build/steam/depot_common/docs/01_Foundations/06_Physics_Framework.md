# 06_Physics_Framework

This document formalizes kinematics, collision detection, and spatial partitioning on the infinite 2D plane.

---

## 1. Kinematics Integration (Euler-Cromer)
Velocity is integrated before position to ensure energy stability:
$$\begin{aligned}
v_{t+1} &= v_t + \left( \frac{F}{M} \right) \Delta t \\
x_{t+1} &= x_t + v_{t+1} \Delta t \\
\omega_{t+1} &= \omega_t + \left( \frac{\tau}{I} \right) \Delta t \\
\theta_{t+1} &= \theta_t + \omega_{t+1} \Delta t
\end{aligned}$$
* **Damping:** $v \leftarrow v \times (1.0 - C_{\text{drag}})$
* **C++ Symbol:** [RigidBodyComponent](file:///C:/Users/ASUS/Documents/Primodilas/Engine/Physics/RigidBodyComponent.hpp).

---

## 2. Spatial Hash Grid Partitioning
$$\text{Key} = (\lfloor x / S \rfloor \ll 32) \mid (\lfloor y / S \rfloor \ \& \ \text{0xFFFFFFFF})$$
* Broadphase queries are $O(N)$ amortized per tick.
* **C++ Symbol:** [SpatialHashGrid](file:///C:/Users/ASUS/Documents/Primodilas/Engine/Physics/SpatialHashGrid.hpp).

---

## 3. Narrowphase Collision (SAT)
* Projects vertices along all edge normals; separating axis means no collision.
* **Impulse Resolution:**
  $$j = \frac{-(1 + e)(v_{\text{rel}} \cdot \hat{n})}{M_A^{-1} + M_B^{-1}}$$
  $$v_A \leftarrow v_A - j M_A^{-1} \hat{n}, \quad v_B \leftarrow v_B + j M_B^{-1} \hat{n}$$
* Slop-adjusted positional correction prevents sinking.
* **C++ Symbol:** [PhysicsSystem](file:///C:/Users/ASUS/Documents/Primodilas/Engine/Physics/PhysicsSystem.hpp).
