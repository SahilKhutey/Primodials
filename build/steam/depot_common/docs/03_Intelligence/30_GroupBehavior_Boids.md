# 30. Boids Flocking — GroupBehaviorSystem

## Overview

`GroupBehaviorSystem` implements Craig Reynolds' classic **Boids** model, providing emergent collective motion for entity swarms, herds, flocks, and crowds. Entities with a `FlockComponent` automatically participate in group steering.

---

## FlockComponent (`Engine/ECS/FlockComponent.hpp`)

```cpp
struct FlockComponent {
    static constexpr u32 TypeId = static_cast<u32>(ComponentTypeId::FlockComponent); // 16

    uint32_t flock_id        = 0;      // Flock membership group
    float perception_radius  = 50.0f;  // Neighbour detection range (world units)
    float separation_weight  = 1.5f;   // Repulsion strength from close neighbours
    float alignment_weight   = 1.0f;   // Velocity matching strength
    float cohesion_weight    = 1.0f;   // Centre-of-mass attraction strength
    float max_speed          = 10.0f;  // Max steering speed applied
};
```

---

## GroupBehaviorSystem (`Engine/Simulation/Systems/GroupBehaviorSystem.h`)

### Three Boids Rules

| Rule | Description | Force Direction |
|---|---|---|
| **Separation** | Steer away from neighbours within `perception_radius / 2` | Away from average close neighbour position |
| **Alignment** | Match average velocity of flock neighbours | Toward average neighbour velocity |
| **Cohesion** | Steer toward centre of mass of flock neighbours | Toward average neighbour position |

### Per-tick Algorithm

```
For each entity E with FlockComponent:
  neighbours = all entities in same flock_id within perception_radius
  
  separation_force = weighted_sum( normalize(E.pos - N.pos) for N too close )
  alignment_force  = normalize( average(N.velocity) - E.velocity )
  cohesion_force   = normalize( average(N.pos) - E.pos )
  
  steering = separation_weight * separation_force
           + alignment_weight  * alignment_force
           + cohesion_weight   * cohesion_force

  E.RigidBody.force += clamp(steering, max_speed)
```

### Priority

Priority `60` — runs after perception systems, before physics integration.

### Usage

```cpp
GroupBehaviorSystem flock_sys(&world);
scheduler.Register(&flock_sys);

// Per entity:
FlockComponent f;
f.flock_id = 1;
f.perception_radius = 80.0f;
f.separation_weight = 2.0f;
world.AddComponent(entity, f);
```

---

## Emergent Behaviours

With default weights, entities naturally exhibit:

- **V-formations** and **murmuration-like waves** when speed alignment is dominant.
- **Tight clustering** when cohesion weight is high.
- **Individual spread** when separation weight is dominant.

Adjusting the three weights per `FlockComponent` allows unique species-level swarm personalities.

---

## Test Coverage

| Test File | Cases |
|---|---|
| `EngineTests/PhaseETests/test_group_behavior.cpp` | Separation repulsion, alignment velocity averaging, cohesion pull toward centroid, multi-flock isolation |
