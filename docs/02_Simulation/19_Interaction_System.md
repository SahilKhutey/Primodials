# 19_Interaction_System

This document specifies the interaction model — how entities affect each other through proximity, field emissions, consumption, combat, trading, and teaching.

---

## 1. Interaction Classification

All entity-to-entity interactions fall into one of five classes:

| Class       | Mechanism          | Examples                                         |
|------------|-------------------|--------------------------------------------------|
| **Physical** | Collision impulse  | Bouncing, crushing, blocking paths               |
| **Field**    | Influence emission | Fear propagation, leadership aura, morale boost  |
| **Consumptive** | Direct transfer | Eating resource nodes; absorbing defeated entities |
| **Social**   | Memory + Trust     | Sharing memory coordinates; building trust bonds  |
| **Instructive** | Teaching       | Expert shapes transmitting skills to novices     |

---

## 2. Consumptive Interaction (Eating)

When a Gather action targets a `ResourceComponent` entity and the entity enters eating range:

```
if distance(agent, resource) < R_eat:
    consumed = min(C_eat_rate × dt, resource.richness)
    resource.richness -= consumed
    agent.energy += consumed × E_conversion_factor
    if resource.richness <= 0:
        commandBuffer.DeferDestroy(resource)
        spawnDecayNode(resource.position, resource.elementalMass)
```

---

## 3. Field Interaction (Influence)

Influence fields are spatially continuous emissions detected via Spatial Hash Grid queries. Each tick, entities scan for `InfluenceComponent` entities within `R_sense`:

$$\text{ReceivedField}_{\text{type}} = \sum_{j \in \text{emitters}} \frac{S_j}{1.0 + d_j^2}$$

Field responses by type:

| Influence Type   | Cognitive Effect                                      |
|-----------------|-------------------------------------------------------|
| `Fear`           | mind.fear += ReceivedField × dt                      |
| `Leadership`     | mind.trust += ReceivedField × 0.1 × dt               |
| `Knowledge`      | Memory log receives emitter's memory table entries   |
| `Morale`         | mind.courage += ReceivedField × 0.05 × dt            |

---

## 4. Social Interaction (Trust & Memory Sharing)

When two entities with high mutual trust scores remain in proximity for sustained ticks, a **social bond** event triggers:
* Both entities exchange top-3 memory entries.
* Trust bond is logged in social graph (future Society System).
* Shared career information may shift department affinities.

---

## 5. Instructive Interaction (Teaching)

Expert (Rank 3) entities can transmit skill boosts to nearby Novice (Rank 1) entities:

$$\Delta S_{\text{student}} = K_{\text{teach}} \times S_{\text{teacher}} \times \Delta t$$

Teaching occurs automatically when:
1. Teacher is performing the relevant task action.
2. Student is within `R_teach` radius.
3. Student's skill level < Teacher's skill level.

This is an emergent form of **cultural transmission** — expertise propagates through the civilization without any explicit teaching command system.

---

## 6. Combat Interaction

When a military-affinity entity's utility selects a Combat action and a target entity is within range:

```
damage = attacker.mass × |v_relative| × G_military × skill_combat
target.health -= damage
attacker.energy -= C_combat × dt
```

Defeated entities (health ≤ 0) drop their mass as a decay resource node. Attackers gain energy from consuming defeated enemies at a lower efficiency than food nodes (reflecting the "arms race energy cost" principle).
