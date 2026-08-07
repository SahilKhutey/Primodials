# 04_Attention_System

This document specifies the attention system — how entities filter perceived candidate nodes to focus on the most salient threats or resources.

---

## 1. Saliency Filters
Perception maps can yield dozens of candidate nodes. The attention layer computes a **Saliency Score** ($S$) for each candidate to determine what the entity focuses on:

$$S = \text{BaseWeight}_{\text{type}} \times \text{Urgency}_{\text{need}} \times \text{ProximityFactor}$$

* **ProximityFactor:** Extenuates close targets using inverse distance squared:
  $$\text{ProximityFactor} = \frac{1.0}{1.0 + d^2}$$
* **Urgency:** Multiplied by internal drives (e.g. Hunger boosts Food saliency; Fear boosts Threat saliency).

---

## 2. Attention Selection
The candidate with the maximum saliency score is designated as the **Primary Focus**:

$$\text{PrimaryFocus} = \arg\max_{c \in \mathcal{P}} S_c$$

All pathing steering forces and interaction behaviors target the Primary Focus.

---

## 3. Cognitive Optimization
By selecting a single Primary Focus, the AI avoids evaluating pathfinding and steering trajectories for all surrounding candidates, preserving tick budgets and avoiding heap allocations during system runs (System Law 5).
