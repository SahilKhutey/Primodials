# Physics

This document specifies kinematics integration, collision detection, and contact resolution rules.

---

## 1. Kinematics Integration (Euler-Cromer)
Velocity is updated before position to ensure energy stability:

$$\begin{aligned}
\mathbf{v}_{t+1} &= \mathbf{v}_t + \left( \frac{\mathbf{F}}{M} \right) \Delta t \\
\mathbf{x}_{t+1} &= \mathbf{x}_t + \mathbf{v}_{t+1} \Delta t \\
\omega_{t+1} &= \omega_t + \left( \frac{\tau}{I} \right) \Delta t \\
\theta_{t+1} &= \theta_t + \omega_{t+1} \Delta t
\end{aligned}$$

---

## 2. Separating Axis Theorem (SAT)
Collision overlaps are checked by projecting polygon vertices onto edge normals:
* If projection spans do not overlap on at least one normal axis, the polygons do not collide.
* If all axes overlap, the axis with the minimum overlap is designated as the collision normal $\hat{n}$.

---

## 3. Contact Resolution
* **Impulse Force:**
  $$j = \frac{-(1 + e)(\mathbf{v}_{\text{rel}} \cdot \hat{n})}{\frac{1}{M_A} + \frac{1}{M_B}}$$
* **Stabilization:** Penetration depth $d$ is corrected using Baumgarte parameters:
  $$\mathbf{x}_{\text{correction}} = \max(d - \text{slop}, 0.0) \times k_{\text{baumgarte}} \times \hat{n}$$
This prevents shapes from sinking into walls or intersecting.
