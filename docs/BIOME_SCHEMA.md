# 🛠️ Primordials — Biome JSON Schema Specification

Biomes in Primordials define the visual palette, initial population balances, and ecological dynamics.

---

## 1. SCHEMA DEFINITION

```json
{
  "$schema": "http://json-schema.org/draft-07/schema#",
  "title": "PrimordialsBiomeConfig",
  "type": "object",
  "required": ["id", "displayName", "description", "colors", "ecology"],
  "properties": {
    "id": {
      "type": "string",
      "pattern": "^[a-z0-9-]+$"
    },
    "displayName": {
      "type": "string"
    },
    "description": {
      "type": "string"
    },
    "colors": {
      "type": "object",
      "required": ["bgInner", "bgOuter", "creaturePalette"],
      "properties": {
        "bgInner": { "type": "string", "pattern": "^#[0-9a-fA-F]{6}$" },
        "bgOuter": { "type": "string", "pattern": "^#[0-9a-fA-F]{6}$" },
        "creaturePalette": {
          "type": "array",
          "items": {
            "type": "array",
            "minItems": 3,
            "maxItems": 3,
            "items": { "type": "number" }
          }
        }
      }
    },
    "ecology": {
      "type": "object",
      "required": ["initialPrey", "initialPredators", "foodRate", "carryingCapacity"],
      "properties": {
        "initialPrey": { "type": "integer", "minimum": 1 },
        "initialPredators": { "type": "integer", "minimum": 0 },
        "foodRate": { "type": "number", "minimum": 0.1 },
        "carryingCapacity": { "type": "integer", "minimum": 50 }
      }
    }
  }
}
```

---

## 2. EXAMPLE CONFIGURATION (`deep-sea.json`)

```json
{
  "id": "deep-sea",
  "displayName": "Deep Sea",
  "description": "Bioluminescent organisms drifting through abyssal ocean trenches.",
  "colors": {
    "bgInner": "#0a1628",
    "bgOuter": "#020812",
    "creaturePalette": [
      [180, 80, 60],
      [200, 75, 55],
      [280, 70, 60],
      [320, 65, 55]
    ]
  },
  "ecology": {
    "initialPrey": 120,
    "initialPredators": 6,
    "foodRate": 1.5,
    "carryingCapacity": 300
  }
}
```
