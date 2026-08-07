# 20_Government_System

This document specifies Sentinel regulatory policies, emergency directives, and alert systems.

---

## 1. Regulatory Policies
Sentinels maintain civic order by switching colony-wide policies in response to statistics trends:
* **Growth Policy:** Active during energy surplus; boosts reproduction weight modifiers.
* **Harvest Policy:** Active during resource deficit; overrides default shape career weights to focus on gathering.
* **Fortify Policy:** Active during threat alarms; directs builders to construct defensive walls.

---

## 2. Dynamic Career Balancing
The Sentinel calculates optimal department sizes and scales policy overrides:

$$\text{RequiredGatherers} = \frac{\text{ColonyStarvationRate}}{\text{AverageHarvestYield}}$$

If RequiredGatherers is high, the Sentinel shifts the policy, converting idle builders and researchers to gatherers.

---

## 3. Civic Alert Levels
The government system manages three alert levels:

| Alert Level | Trigger Condition | Mechanical Action |
|-------------|-------------------|-------------------|
| **Green**   | Threat field = 0  | Standard operations |
| **Yellow**  | Threat field > 20 | Soldiers patrol; civilians gather near storage |
| **Red**     | Threat alarm cast | All shapes withdraw to Sentinel command center |
