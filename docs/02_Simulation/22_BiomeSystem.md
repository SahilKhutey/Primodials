# 22. Biome System

## Overview

`BiomeSystem` provides **spatial environmental classification** of the simulation world. Any world position can be queried for its biome type and the environmental modifiers that biome applies.

---

## Biome Types

The simulation world is divided into 5 spatial biomes mapped by a 2D Voronoi-like region partition:

| Biome | Fertility | Moisture | Terrain Friction | Description |
|---|---|---|---|---|
| `LushForest` | 1.5× | 0.8 | 0.9 | Dense vegetation, rich resources, slow movement |
| `AridDesert` | 0.3× | 0.1 | 0.4 | Scarce resources, fast movement, extreme heat |
| `Tundra` | 0.4× | 0.5 | 0.7 | Cold, sparse growth, moderate friction |
| `VolcanicCrater` | 0.2× | 0.2 | 1.5 | Dangerous terrain, very low fertility |
| `DeepOcean` | 0.6× | 1.0 | 2.0 | Aquatic movement penalty, moderate fertility |

---

## BiomeProperties

Returned by `BiomeSystem::get_properties_at(position)`:

```cpp
struct BiomeProperties {
    BiomeType type;
    float fertility_multiplier;  // Multiplier on resource yield
    float moisture;              // 0.0 to 1.0
    float terrain_friction;      // Multiplier on entity movement drag
};
```

---

## Public API

```cpp
BiomeSystem biome_sys;

// Query the biome properties at any world position
auto props = biome_sys.get_properties_at({x, y});

// Get the raw biome classification
BiomeType b = biome_sys.get_biome_at({x, y});
```

---

## Integration with Other Systems

### ResourceSystem
Scale resource node yield by biome fertility:
```cpp
float yield = base_yield * biome.fertility_multiplier;
```

### PhysicsSystem
Apply terrain friction to entity linear drag:
```cpp
rb.linear_damping = base_damping * biome.terrain_friction;
```

### WeatherSystem
Biome amplifies weather effects — desert biomes intensify Heatwave metabolic costs; Tundra biomes intensify Blizzard impacts.

---

## Priority

Priority: `-35` — runs immediately after WeatherSystem, before simulation tick systems. Ensures biome properties are stable throughout the tick.

---

## Test Coverage

| Test File | Cases |
|---|---|
| `EngineTests/PhaseETests/test_biome.cpp` | Position-to-biome mapping, fertility/moisture/friction correctness per biome, boundary transitions |
