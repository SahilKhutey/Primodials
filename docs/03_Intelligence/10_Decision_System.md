# 10_Decision_System

This document specifies the Utility AI decision system — the core algorithm evaluating and selecting actions.

---

## 1. Utility AI Evaluation
Every decision tick, the [OptimizationSystem](file:///C:/Users/ASUS/Documents/Primodilas/Engine/AI/OptimizationSystem.cpp) evaluates utility equations for all action candidates:

$$\text{SelectedAction} = \arg\max_{a \in \mathcal{A}} U_a$$

---

## 2. Candidate Utility Equations

### Gather Utility
$$U_{\text{Gather}} = \text{Hunger} \times 10.0 \times \left( \frac{1.0}{1.0 + 0.02 d^2} \right) \times G_{\text{survival}} \times S_{\text{survival}}$$
* Scales with hunger need and closeness to targets (sight or memory).

### Explore Utility
$$U_{\text{Explore}} = 1.5 \times G_{\text{explore}} \times S_{\text{exploration}} + (C_{\text{wonder}} \times W \times 0.1)$$
* Driven by exploration genes and curiosity values.

### Reproduce Utility
$$U_{\text{Reproduce}} = \max(0.0, E_m - E_{\text{threshold}}) \times 1.5 \times G_{\text{reproduce}}$$
* Only active when metabolic energy reserves are high.

### Escape Utility
$$U_{\text{Escape}} = \text{Fear} \times 1.2$$
* Scales with local threat fields and raw fear emotions.

---

## 3. Decision Hysteresis (Anti-Oscillation)
To prevent shapes from rapidly switching between two actions of similar utility (e.g. Gather vs. Explore), decisions use a hysteresis timer:

```cpp
if (ai.actionTimer > 0.0f) {
    ai.actionTimer -= dt;
    return; // Retain current action
}
```

When a new action is selected, the action timer is set to `ACTION_LOCK_TIME` (default: $0.5$ seconds), ensuring behavioral stability.
