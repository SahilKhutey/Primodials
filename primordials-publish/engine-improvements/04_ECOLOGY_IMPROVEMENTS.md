# 🧬 PRIMORDIALS — Ecological Dynamics Improvements

---

## 1. Runge-Kutta 4th-Order (RK4) Differential Solver

Replaces 1st-order Forward Euler integration with 4th-order Runge-Kutta integration:
- Reduces accumulation drift by **$O(\Delta t^4)$**.
- Allows simulation timesteps ($\Delta t$) up to $4\times$ larger without introducing numerical instability or population explosions.

---

## 2. Carrying Capacity Logistic Self-Limitation

Adds non-linear logistic damping to prey growth ($\gamma x^2$) to simulate resource depletion, preventing unbounded exponential spirals:

$$\frac{dx}{dt} = \alpha x - \beta x y - \gamma x^2$$
$$\frac{dy}{dt} = \delta x y - \epsilon y$$

---

## 3. Equilibrium & Stability Eigenvalue Verification

Provides compile-time and runtime Jacobian eigenvalue checks to ensure chosen biome species coefficients yield sustainable limit cycles rather than catastrophic extinction cascades.
