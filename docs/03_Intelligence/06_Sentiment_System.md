# 06_Sentiment_System

This document specifies the sentiment system — how entities develop and track individual-to-individual relationships and reputational standing.

---

## 1. Social Sentiment Values
Entities record social standing parameters relative to encountered neighbors:

```cpp
struct SocialRelation {
    Entity target;
    f32    trust;       // range: -100.0 (hostile) to +100.0 (blood bond)
    f32    respect;     // range: 0.0 to 100.0 (career skill weight)
    f32    interactions;// total interaction tick counter
};
```

---

## 2. Dynamic Sentiment Adjustments
Relationship parameters shift based on transactional events:
* **Successful trade / coordinate share:** $\Delta \text{trust} = +5.0$.
* **Cooperative combat (fighting same threat):** $\Delta \text{trust} = +10.0$.
* **Accidental physics collision:** $\Delta \text{trust} = -2.0$ (scaled down if restitution is high).
* **Direct combat attack:** $\Delta \text{trust} = -50.0$.

---

## 3. Reputational Alignment
Sentinels aggregate individual relations to define **Faction Standing**:
$$\text{FactionStanding} = \frac{1}{N} \sum_{i=1}^N \text{trust}_i$$
If FactionStanding drops below $-50.0$, the Sentinel flags all members of the target colony as hostiles, redirecting military coordinate missions.
