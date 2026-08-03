# 07_Chemistry_Framework

This document outlines the chemistry layer representing elemental compositions, reaction rates, and molecular state equations.

---

## 1. Primary Elements & CHONPS
The simulation tracks six foundational elements necessary for biochemical reactions:
* **Carbon (C):** Base structure element of all physical geometries.
* **Hydrogen (H) & Oxygen (O):** Form water ($H_2O$) necessary for respiration and fluid diffusion.
* **Nitrogen (N) & Phosphorus (P):** Enforced in DNA genetics replication and energy carrier transfer ($ATP$).
* **Sulfur (S):** Stabilizes structural geometry protein links.

---

## 2. Dynamic Fluid State
* **Solubility:** Compounds diffuse across the 2D plane based on spatial grid cell density gradients:
  $$J = -D \nabla \phi$$
  where $J$ is the diffusion flux, $D$ is the diffusion coefficient, and $\phi$ is the chemical concentration.
* **Friction and Viscosity:** Chemical solutions alter fluid resistance, modifying velocity damping factors for moving entities.

---

## 3. Reaction Kinetics & Respiration
* **Respiration Formula:** Shapes burn carbon and oxygen to replenish metabolic energy ($E$), releasing carbon dioxide ($CO_2$):
  $$C_6H_{12}O_6 + 6O_2 \rightarrow 6CO_2 + 6H_2O + \text{Energy}$$
* **Thermal Modifiers:** Chemical reaction rates are scaled by local cell temperatures using the Arrhenius equation:
  $$k = A e^{-\frac{E_a}{R T}}$$
  Extremely low or high temperatures cause metabolic processes to stop, resulting in cell decay and death.
