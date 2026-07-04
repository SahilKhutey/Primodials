# 12_Learning_System

This document specifies the learning system — how skill levels increment through task repetition and propagate through peer interactions.

---

## 1. Practice Skill Accumulation
Performing actions increments the corresponding skill value in [CareerComponent](file:///C:/Users/ASUS/Documents/Primodilas/Engine/ECS/CareerComponent.hpp) by a rate of $0.1 \times \Delta t$ per second:

$$\text{Skill}_{\text{type}} \leftarrow \text{Skill}_{\text{type}} + 0.1 \times dt$$

* **Gathering** trains `skillSurvival`.
* **Exploring** trains `skillExploration`.
* **Building** trains `skillConstruction`.

---

## 2. Dynamic Learning Multipliers
Skill increments are modified by genes and age:
$$\Delta \text{Skill} = 0.1 \times G_{\text{science}} \times M_{\text{age}} \times dt$$
* **Science Gene ($G_{\text{science}}$):** High values accelerate learning speeds.
* **Age Modifier ($M_{\text{age}}$):** Infants and adults learn at full speed ($1.0$), while senescent shapes experience learning speed decays.

---

## 3. Teaching Propagation
Novice shapes within range of Expert shapes receive direct skill transfers:
$$\Delta \text{Skill}_{\text{novice}} = 0.05 \times \text{Skill}_{\text{expert}} \times dt$$
This allows career specializations to spread culturally through the colony population.
