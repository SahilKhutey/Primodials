# 23_Military_System

This document specifies squad formations, coordinate defense patrols, and threat mitigation steering behaviors.

---

## 1. Squad Formations
Military shapes (Triangles) form protective groups called **Squads**:
* A Leader Soldier is selected based on maximum combat skill.
* Follower Soldiers align using cohesive flocking steering weights:
  $$\mathbf{F}_{\text{flock}} = w_1 \mathbf{F}_{\text{cohesion}} + w_2 \mathbf{F}_{\text{alignment}} + w_3 \mathbf{F}_{\text{separation}}$$
* Squads maintain a delta-offset wedge formation to maximize combat collision potential.

---

## 2. Defensive Patrols
Sentinels assign patrol coordinates along trade route graphs:
* Squads cycle through waypoint coordinates, scanning for hostile shapes or predators.
* **Aggressive Intercept:** If a threat is detected, the squad breaks formation and intercept-steers to engage, resolving through kinetic collision damage (see [19_Interaction_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/02_Simulation/19_Interaction_System.md)).

---

## 3. Threat Mitigation Vectors
When combat concludes, soldiers return to the local Sentinel to restore health and energy. If a colony's military strength falls below a safety threshold, the Sentinel locks defensive policies (Green to Red alerts) to protect building infrastructures.
