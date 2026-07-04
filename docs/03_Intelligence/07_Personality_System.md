# 07_Personality_System

This document specifies the personality system — how DNA genes define baseline cognitive traits and behavioral variance.

---

## 1. Genetic Baselines
While emotional states fluctuate dynamically, an entity's core **Personality Profile** is defined by its [GeneticsComponent](file:///C:/Users/ASUS/Documents/Primodilas/Engine/ECS/GeneticsComponent.hpp). 

Genes 4 through 9 represent behavioral affinities:
* **Gene 4 (Survival):** Modulates baseline hunger fear responses.
* **Gene 5 (Engineering):** Modulates utility drive for construction and repairs.
* **Gene 6 (Science):** Modulates memory retention and learning speeds.
* **Gene 7 (Economy):** Modulates transport priority weights.
* **Gene 8 (Military):** Modulates rage thresholds and combat aggression.
* **Gene 9 (Exploration):** Modulates baseline wonder triggers.

---

## 2. Emotional Response Modifiers
Personality traits scale how entities respond to emotional stimulations:
$$\Delta \text{Fear} = \text{ThreatStrength} \times (1.5 - G_{\text{military}})$$
* Entities with high Military genes ($G_{\text{military}}$) accumulate Fear significantly slower.
* Entities with high Exploration genes accumulate Wonder faster, prompting exploratory migrations.

---

## 3. Phenotypic Personality Variance
Due to random mutations during birth replication (see [13_Genetics_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/02_Simulation/13_Genetics_System.md)), individuals within the same colony exhibit distinct personalities. This diversity protects the civilization: bold explorers discover new sectors, while cautious builders focus on logistics.
