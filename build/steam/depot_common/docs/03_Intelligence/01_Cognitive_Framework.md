# 01_Cognitive_Framework

This document specifies the unified cognitive pipeline that governs all intelligent shapes.

---

## 1. Unified Cognitive Pipeline

Every intelligent entity runs a standardized cognitive update loop:

```
Perception ──→ Attention ──→ Memory ──→ Emotion ──→ Reason ──→ Plan ──→ Decision ──→ Action ──→ Learn
```

1. **Perception:** Query Spatial Hash Grid cells within sensory radius.
2. **Attention:** Filter perceived objects by saliency (threat proximity, nutrient richness).
3. **Memory:** Record new coordinates, decay old coordinate relevance.
4. **Emotion:** Fluctuating variables (Fear, Rage, Trust) derived from sensory inputs.
5. **Reason & Plan:** Compute actions' utility values based on needs and interests.
6. **Decision:** Select action with the maximum utility value.
7. **Action:** Apply kinematic force thrust vectors to physics engine.
8. **Learn:** Increment career skills derived from the executed action type.

---

## 2. Component Structures

The cognitive system is powered by two primary components:

* **C++ Symbol:** [MindComponent](file:///C:/Users/ASUS/Documents/Primodilas/Engine/ECS/MindComponent.hpp)
  Stores needs (hunger, fatigue), emotional values (fear, trust), and memory log records.
* **C++ Symbol:** [AIComponent](file:///C:/Users/ASUS/Documents/Primodilas/Engine/ECS/AIComponent.hpp)
  Stores active action states, target positions, pathing vectors, and timers.

---

## 3. Cognitive Performance Constraints

To prevent cognitive processing from dominating tick budgets at high entity scales, the scheduler utilizes **LOD Tick Decoupling**:
* **Tier 1 (High Priority):** Entities within view of the camera execute the cognitive pipeline every tick (60 Hz).
* **Tier 2 (Medium Priority):** Off-screen entities update cognitive pipelines every 5 ticks (12 Hz).
* **Tier 3 (Dormant):** Distant entities halt cognitive planning, freezing active action states until re-entered by other shapes.
