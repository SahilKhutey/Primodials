# Mathematics

This document specifies the vector and geometry mathematics used in all physics and simulation systems.

---

## 1. Vector Operations
The engine uses a custom math module:
* **Vector2 Dot Product:**
  $$\mathbf{a} \cdot \mathbf{b} = a_x b_x + a_y b_y$$
* **Vector2 Rotation:**
  $$\begin{aligned}
  x' &= x \cos(\theta) - y \sin(\theta) \\
  y' &= x \sin(\theta) + y \cos(\theta)
  \end{aligned}$$

---

## 2. Polygon Geometry
* **Gauss Area (Shoelace Formula):**
  $$\text{Area} = \frac{1}{2} \left| \sum_{i=0}^{n-1} (x_i y_{i+1} - x_{i+1} y_i) \right|$$
* **Centroid coordinates:**
  $$\begin{aligned}
  C_x &= \frac{1}{6 A} \sum_{i=0}^{n-1} (x_i + x_{i+1}) (x_i y_{i+1} - x_{i+1} y_i) \\
  C_y &= \frac{1}{6 A} \sum_{i=0}^{n-1} (y_i + y_{i+1}) (x_i y_{i+1} - x_{i+1} y_i)
  \end{aligned}$$
* **Moment of Inertia ($I$):**
  $$I = \frac{M}{6} \frac{\sum \left( |x_i y_{i+1} - x_{i+1} y_i| (r_i^2 + r_i \cdot r_{i+1} + r_{i+1}^2) \right)}{\sum |x_i y_{i+1} - x_{i+1} y_i|}$$

---

## 3. Probability & Pseudo-Randomness
Random number generation is managed by LCG state loops:
$$\text{state}_{t+1} = (a \times \text{state}_t + c) \mod m$$
This guarantees identical random values across compiler target platforms when initialized with the same seed.
