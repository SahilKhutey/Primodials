# 25_Technology_System

This document specifies the global technology tree, research branches, and trait modifiers.

---

## 1. The Technology Tree Nodes
Civilizations unlock technology nodes using accumulated Science Points:

```
[Metallurgy] ─────→ [Advanced Weapons] (combat damage +20%)
     │
     ├───────────→ [Stone Roads] (road speed modifier +15%)
     │
[Architecture] ──→ [Workshops] (crafting speed +25%)
```

---

## 2. Modifying Phenotypic Invariants
Unlocking tech nodes applies multipliers to corresponding component systems:
* **Metallurgy:** Reduces tool fabrication times in workshops.
* **Weapons:** Adds $0.20$ to the combat skill multipliers of all squad soldiers.
* **Friction Drag:** Decreases drag coefficients for carrier shapes:
  $$C_{\text{drag}} \leftarrow C_{\text{drag}} \times 0.90$$

---

## 3. Technology Integration Interface
When a tech is unlocked, the Sentinel propagates updates through its `Morale` influence field. Shapes entering the field query the Sentinel for active tech flags, applying the modifiers directly to their phenotype calculations.
