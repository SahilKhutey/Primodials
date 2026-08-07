# 09_Production_System

This document specifies how raw resources are transformed into refined goods, buildings, and technology outputs through a production pipeline.

---

## 1. Production Philosophy

Raw resources → Refined goods → Infrastructure → Technology upgrades.

Production is not instantaneous — it takes simulation ticks and consumes energy, creating meaningful time-cost trade-offs that drive specialization.

---

## 2. Production Stages

```
Stage 0: Raw Extraction
  (Gatherers harvest Food, Minerals, Water from Resource Nodes)
       │
       ▼
Stage 1: Refining
  (Engineers convert Minerals → Metal Bars, Wood → Planks)
       │
       ▼
Stage 2: Fabrication
  (Builders consume refined goods → construct Buildings)
       │
       ▼
Stage 3: Research
  (Sentinels consume Knowledge → unlock Technology Nodes)
```

---

## 3. Production Recipe Format

```cpp
struct ProductionRecipe {
    ResourceType   input[4];     // Required input types
    f32            inputQty[4];  // Required input quantities
    ResourceType   output;       // Produced output type
    f32            outputQty;    // Produced output quantity
    f32            productionTime; // Ticks required
    u32            requiredTech;   // Prerequisite technology ID (0 = none)
};
```

**Example Recipes:**

| Input                    | Output        | Ticks | Required Tech |
|--------------------------|---------------|-------|---------------|
| 5 Minerals + 2 Energy    | 1 Metal Bar   | 30    | None          |
| 3 Metal Bars + 1 Water   | 1 Tool        | 60    | Metallurgy    |
| 10 Tools + 5 Wood Planks | 1 Workshop    | 120   | Construction  |

---

## 4. Efficiency Modifiers

Production speed and yield are modified by:
* **Worker Skill:** An Engineer at Rank 3 (Expert) produces 40% faster than Rank 1 (Citizen).
* **Building Bonus:** Workshops increase adjacent production efficiency by 25%.
* **Technology Multipliers:** Researched technologies apply global production speed bonuses.

$$\text{Production Rate} = R_{\text{base}} \times M_{\text{skill}} \times M_{\text{building}} \times M_{\text{tech}}$$

---

## 5. Production Bottlenecks & Emergent Shortages

The production pipeline is the primary driver of economic emergent behavior:
* A mineral vein exhausted without alternative supply halts metal production.
* Halted metal production prevents tool fabrication.
* Missing tools collapse Workshop construction.
* Without Workshops, knowledge generation slows.

This cascading dependency chain emerges without scripting — it is a natural consequence of the resource-production-technology dependency graph.
