# AI

This document specifies the sensory perception queries, memory log decay models, and Utility AI decisions.

---

## 1. Grid Perception & Saliency Attention
* **Grid Sweep:** Query the Spatial Hash Grid for entity targets within radius $R_{\text{sense}}$.
* **Saliency Priority:** Filter candidates by distance and urgency:
  $$S = \text{BaseWeight} \times \text{Urgency}_{\text{need}} \times \left(\frac{1.0}{1.0 + d^2}\right)$$

---

## 2. Memory Log Decay
Locations are cached in a ring buffer. The relevance strength of coordinates decays exponentially:

$$\text{strength}_{t+1} = \text{strength}_t \times e^{-\lambda \cdot \Delta t}$$

* High Science affinity genes ($G_{\text{science}}$) decrease decay rate constants $\lambda$, preserving memory longer.

---

## 3. Utility AI Drive Equations
Utility AI evaluates action candidates during decision updates:
* **Gather Utility:**
  $$U_{\text{Gather}} = \text{Hunger} \times 10.0 \times \left(\frac{1.0}{1.0 + 0.02 d^2}\right) \times G_{\text{survival}}$$
* **Escape Utility:**
  $$U_{\text{Escape}} = \text{Fear} \times 1.2$$
The action yielding the maximum utility score is selected for execution.
