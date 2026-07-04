# 08_Motivation_System

This document specifies the motivation system — how physiological needs are translated into urgent cognitive drives.

---

## 1. Physiological Needs
Every dynamic entity tracks needs in the range $0.0$ (fully satisfied) to $100.0$ (critical deficiency):
* **Hunger ($H_{\text{need}}$):** Derived from metabolic energy depletion:
  $$H_{\text{need}} = 100.0 - E_m$$
* **Fatigue ($F_{\text{need}}$):** Accumulates over active execution ticks, reset by resting or remaining stationary:
  $$F_{\text{need}, t+1} = F_{\text{need}, t} + C_{\text{exhaust}} \cdot \Delta t$$
* **Instability ($I_{\text{need}}$):** Accumulates when receiving structural damage, reset by repair behaviors.

---

## 2. Motivation Curves (Homeostasis)
Needs do not scale drives linearly. To prioritize critical urgencies, drives follow exponential homeostasis curves:

$$\text{Drive}_{\text{hunger}} = \left( \frac{H_{\text{need}}}{100.0} \right)^k$$

* **Exponent ($k$):** Set to $2.0$. At low hunger (50%), the drive is only $0.25$ (allowing other tasks to execute). At high hunger (90%), the drive spikes to $0.81$, overriding other motivations.

---

## 3. Drive Consolidation
The AI planner evaluates all drives each cycle to determine the dominant motivation:
$$\text{DominantMotivation} = \max(\text{Drive}_{\text{hunger}}, \text{Drive}_{\text{fatigue}}, \text{Drive}_{\text{defense}})$$
This dictates the prioritizations of action utility calculations.
