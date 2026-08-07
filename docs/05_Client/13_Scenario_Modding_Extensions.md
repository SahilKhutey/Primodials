# 13_Scenario_Modding_Extensions

This document specifies modding directories, JSON schemas for custom shapes, and API extensions.

---

## 1. Modding Folder Layout
The client scans a dedicated modding folder at launch:

```
Primodilas (Root)
 └── Mods/
      ├── custom_shapes.json  — User-defined geometry properties
      ├── custom_scenarios/   — Custom starting map profiles
      └── custom_textures/    — Custom visual assets
```

---

## 2. Custom Shape Schema
Modders can define custom shape classes with specific base traits:

```json
{
  "shapeName": "Octagram Defender",
  "baseVertices": 8,
  "defaultColor": "#FF3F00",
  "startingAffinity": {
    "military": 3.5,
    "survival": 0.5
  }
}
```

---

## 3. Sandboxing Boundaries
To prevent malicious mod scripts from crashing host OS files:
* Mods cannot execute arbitrary DLL compilation calls.
* Modifications are restricted to editing database schemas (JSON variables) and visual asset overrides (images, fonts).
