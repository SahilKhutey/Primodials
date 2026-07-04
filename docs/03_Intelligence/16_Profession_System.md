# 16_Profession_System

This document specifies the individual professions — Gatherer, Builder, Soldier, and Scholar — and their behavioral rules.

---

## 1. Professional Profiles

### Gatherer (Survival)
* **Goal:** Maximize food harvesting and water collection.
* **Yield Modifier:** $\text{HarvestAmount} = \text{BaseYield} \times S_{\text{survival}}$.

### Builder (Engineering)
* **Goal:** Construct workshops, storage nodes, and stone roads.
* **Speed Modifier:** $\text{BuildTicks} = \frac{\text{BaseTicks}}{S_{\text{construction}}}$.

### Soldier (Military)
* **Goal:** Guard territories, patrol roads, and eliminate threat emitters.
* **Force Modifier:** $\text{CombatDamage} = M \times |v| \times S_{\text{combat}}$.

### Scholar (Science)
* **Goal:** Research tech tree nodes and maintain Sentinel coordinate maps.

---

## 2. Professional Efficiency Modifiers
The following matrix shows how skill levels modify task yields:

| Skill Level | Harvest Speed | Construct Cost | Combat Power | Research Yield |
|-------------|---------------|----------------|--------------|----------------|
| Apprentice  | 1.0           | 1.0            | 1.0          | 1.0            |
| Journeyman  | 1.25          | 0.90           | 1.30         | 1.20           |
| Master      | 1.50          | 0.75           | 1.60         | 1.40           |
