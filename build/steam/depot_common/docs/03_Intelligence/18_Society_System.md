# 18_Society_System

This document specifies the social networks, reputation graphs, and emergent colony groupings.

---

## 1. Reputation Graphs
The society layer is modeled as a directed graph where nodes represent entities and edges represent relationships:

```
Entity A ──(Trust: +75)──→ Entity B
    ▲                        │
    │                        ▼
 (Trust: -10) ◄────── Entity C (Sentinel)
```

Relationships are analyzed by Sentinels to identify sub-colony cliques.

---

## 2. Colony Core Formation
Colonies emerge when entities cluster around shared storage nodes:
* A Sentinel acts as the regulatory leader of the colony.
* The Sentinel projects a `Leadership` influence field, raising the trust of all shapes inside its command radius.
* This high trust enables cooperation, sharing of resource memories, and division of labor.

---

## 3. Social Cohesion Modifiers
Colony stability relies on maintaining trust values:
* **High Morale:** Sentinel matches target policy goals, boosting collective efficiency.
* **Low Morale:** Frequent resource deficits decay trust, triggering sub-clique divisions and emigration from the colony.
