# 08_Resource_System

This document specifies the resource model — all types of resources, their properties, depletion, regeneration, and interaction with biological and economic systems.

---

## 1. Resource Classification

All resources are entities with a `ResourceComponent`:

```cpp
struct ResourceComponent {
    ResourceType type;       // Food, Mineral, Water, Energy, Knowledge
    f32          richness;   // Current quantity (0.0 – max_richness)
    f32          maxRichness;
    f32          regenRate;  // Units replenished per tick
    f32          depletionRate; // Reduction per consumption event
    bool         isRenewable;
};
```

**Resource Types:**

| Type        | Renewable | Primary Use               | Biome Sources           |
|------------|-----------|---------------------------|-------------------------|
| Food        | Yes       | Metabolic energy           | Forest, Grassland       |
| Mineral     | No        | Construction, Technology   | Desert, Volcanic        |
| Water       | Yes       | Hydration, Cooling         | Ocean, Grassland, Rain  |
| Solar Energy| Yes       | Power for buildings         | Desert (high yield)     |
| Knowledge   | Yes       | Technology tree research   | Accumulated by Sentinels|

---

## 2. Resource Node Spatial Distribution

Resources follow spatial distributions biased by biome fertility:

$$P(\text{node at cell}(i,j)) \propto \text{Fertility}(i,j) \times (1 - \text{Density}_{N}(i,j))$$

where $\text{Density}_{N}$ is the count of existing resource nodes in the neighborhood radius $N$. This creates natural clustering with spacing gaps — not uniform coverage.

---

## 3. Depletion and Regeneration

**Depletion per consumption event:**
$$\text{Richness} \leftarrow \text{Richness} - C_{\text{consume}} \times \text{Entity.Mass}$$

**Regeneration (per tick, if renewable):**
$$\text{Richness} \leftarrow \min(\text{Richness} + R_{\text{regen}} \times \text{Fertility}, \text{MaxRichness})$$

**Exhaustion:** When Richness ≤ 0, the resource entity is destroyed. Its remaining elemental composition is dispersed as trace amounts into the cell's soil, gradually increasing Fertility for future regrowth.

---

## 4. Resource Discovery & Memory

Entities do not know resource locations by default. Resources must be **discovered** through active exploration:
* Entities with `AIAction::Explore` scan their perception radius for `ResourceComponent` entities.
* On discovery, the resource's position and richness are written to the entity's `MindComponent.memories`.
* Sentinels broadcast known resource coordinates to nearby shapes (see [14_Information_Framework](file:///C:/Users/ASUS/Documents/Primodilas/docs/01_Foundations/14_Information_Framework.md)).

---

## 5. Strategic Resource Scarcity

Non-renewable resources (minerals) create long-term strategic pressure:
* Early civilizations rapidly consume surface mineral deposits.
* Advanced civilizations must develop **mining technology** (unlocked via Research System) to extract deeper veins.
* Mineral depletion without research investment causes infrastructure collapse — a natural "Bronze Age Collapse" analogue emerging from resource dynamics alone.
