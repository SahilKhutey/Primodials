# 04_Environment_System

This document specifies the environment — the abiotic world that living entities inhabit. This includes terrain, resource nodes, biome zones, and global environmental state variables.

---

## 1. Environment as a Grid

The simulation environment is represented as a **sparse grid** of cells overlaid on the infinite 2D plane. Only cells containing active features (resources, terrain, temperature gradients) are maintained in memory.

```
EnvironmentCell {
    temperature   : f32     // Degrees (normalized 0.0–1.0)
    humidity      : f32     // Water content (0.0–1.0)
    fertility     : f32     // Resource regeneration rate multiplier
    elevation     : f32     // Determines fluid flow direction
    biomeType     : BiomeID // Desert, Forest, Tundra, Ocean, etc.
    resourceNodes : List<Entity>
}
```

**Cell Size:** Configurable (default: 50 world units × 50 world units).

---

## 2. Biome Types

| Biome      | Temp Range   | Humidity | Fertility | Primary Resources        |
|-----------|--------------|----------|-----------|--------------------------|
| Forest     | 0.4 – 0.7   | High     | 1.5×      | Food, Wood               |
| Desert     | 0.8 – 1.0   | Very Low | 0.3×      | Minerals, Solar Energy   |
| Tundra     | 0.0 – 0.2   | Low      | 0.5×      | Ice, Stone               |
| Grassland  | 0.3 – 0.6   | Medium   | 1.0×      | Food, Water              |
| Ocean      | 0.3 – 0.6   | Max      | 0.8×      | Water, Fish              |
| Volcanic   | 0.9 – 1.0   | Very Low | 0.2×      | Rare Minerals, Geothermal|

Biome type is determined at world generation and changes slowly over geological time (continental drift is a future feature).

---

## 3. Resource Node Spawning

Resources are spawned at world generation and replenish according to environmental fertility:

$$R_{\text{spawn\_rate}} = R_{\text{base}} \times \text{Fertility} \times (1.0 - \text{Occupancy})$$

where Occupancy is the fraction of nearby cells already containing resource nodes.

**Depletion:** When a resource node is consumed to zero richness, it is destroyed. Its elemental composition is transferred to the consuming entity (conservation of mass — see [03_Universe_Principles](file:///C:/Users/ASUS/Documents/Primodilas/docs/01_Foundations/03_Universe_Principles.md)).

**Respawn:** After a biome-defined respawn delay, a new resource node may regenerate in the same cell if no entity occupies the spot.

---

## 4. Environmental Influences on Entities

| Environment Variable | Effect on Entities                                          |
|---------------------|-------------------------------------------------------------|
| High Temperature     | Increases metabolic rate; accelerates starvation           |
| Low Temperature      | Slows chemical reactions; reduces movement speed           |
| High Humidity        | Enhances food resource density                             |
| Low Humidity         | Increases thirst urgency (future hydration need system)    |
| High Fertility       | More frequent resource node regeneration                   |

---

## 5. Environmental Update Frequency

Environment cells update at a lower frequency than entity systems to reduce computational overhead:

| System             | Update Frequency |
|-------------------|-----------------|
| Entity Physics     | Every tick (60 Hz) |
| Influence Fields   | Every tick      |
| Resource Nodes     | Every 10 ticks  |
| Biome Temperature  | Every 60 ticks  |
| Terrain Fertility  | Every 300 ticks |
