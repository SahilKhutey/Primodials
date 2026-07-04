# 08_Scenario_System_Architecture

This document specifies the scenario configuration schemas, starting properties, and validation checks.

---

## 1. Scenario Configuration Format
Scenarios are loaded from external configuration profiles specifying the initial state:

```json
{
  "scenarioName": "Base Artificial Life Sandbox",
  "worldSeed": 987654321,
  "defaultBiome": "Grassland",
  "initialEntities": [
    { "archetype": "Sentinel", "x": 0.0, "y": 0.0 },
    { "archetype": "BasicShape", "vertices": 3, "count": 25 },
    { "archetype": "BasicShape", "vertices": 4, "count": 15 },
    { "archetype": "ResourceNode", "type": "Food", "count": 50 }
  ]
}
```

---

## 2. Dynamic Starting Triggers
Some scenarios define starting scripts (e.g. "Ice Age Survival" spawns a global temperature drop trigger at tick 1,000).

---

## 3. Validation Invariants
The scenario parser validates starting coordinates:
* Entities must not spawn overlapping coordinates (satisfying SAT collision constraints).
* Sentinel entities must be placed at least $50.0$ units apart to prevent starting territorial disputes.
