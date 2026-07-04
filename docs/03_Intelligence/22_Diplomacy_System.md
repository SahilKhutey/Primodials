# 22_Diplomacy_System

This document specifies the diplomacy system — how independent colonies manage inter-faction trust, treaties, and conflicts.

---

## 1. Faction Relation States
Sentinels track diplomatic standings with other Sentinel factions:

| Relation State | Trust Threshold | Policy Effect                                    |
|----------------|-----------------|--------------------------------------------------|
| **Allied**     | Trust $\ge 50.0$| Shared maps; open border trade; defensive pacts   |
| **Neutral**    | $-30.0 < T < 50$| Standard trade tariffs; no military intervention|
| **Hostile**    | Trust $\le -30.0$| Closed borders; trade embargoes; military attacks|

---

## 2. Diplomatic Treaties
When Faction Trust remains high, Sentinels negotiate treaties:
* **Resource Treaty:** Triggers carriers to balance storage levels between both colonies' depots.
* **Technology Pact:** Synchronizes Tech Databases automatically.

---

## 3. Conflict Escalation
Diplomatic trust drops when:
* A colony's shapes trespass into another colony's Voronoi territory cell boundaries (see [11_Space_Framework](file:///C:/Users/ASUS/Documents/Primodilas/docs/01_Foundations/11_Space_Framework.md)).
* Shapes compete over the same mineral veins.
* Accidental physics collisions occur.
If trust drops below $-30.0$, the relationship shifts to Hostile, initiating a state of military war.
