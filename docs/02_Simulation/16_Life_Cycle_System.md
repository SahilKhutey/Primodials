# 16_Life_Cycle_System

This document specifies the complete individual lifecycle — from birth through growth, maturation, aging, death, and decay — with exact equations for each transition.

---

## 1. Lifecycle State Machine

```
SPAWN (LifecycleComponent initialized)
  │
  ▼
INFANT       (age < maturation_age)
  │           — reduced speed, size scaled from genome
  ▼
ADULT        (maturation_age ≤ age < senescence_age)
  │           — full capability; reproduction enabled
  ▼
SENESCENT    (age ≥ senescence_age)
  │           — gradual stat penalties; mortality hazard rises exponentially
  ▼
DYING        (health → 0)
  │           — final tick; decay resource spawned
  ▼
DEAD         — entity destroyed; resource node created
```

---

## 2. Growth Curve (Infant Phase)

During the infant phase, physical size scales up from 50% to 100% of adult genome-defined radius:

$$R_{\text{current}} = R_{\text{adult}} \times \left(0.5 + 0.5 \times \frac{\text{age}}{\text{maturation\_age}}\right)$$

Mass scales proportionally: $M = \rho \times A(R_{\text{current}})$.

Infants have reduced collision damage output and reduced sensory range, making them vulnerable without parental proximity (Sentinel protection fields cover infants).

---

## 3. Aging & Senescence

Aging applies progressive stat debuffs after the senescence threshold:

$$\text{age\_factor} = \frac{\text{age} - T_{\text{sen}}}{T_{\text{max}} - T_{\text{sen}}} \in [0.0, 1.0]$$

| Stat               | Senescent Modifier                          |
|-------------------|---------------------------------------------|
| Max Speed          | $\times (1.0 - 0.4 \times \text{age\_factor})$ |
| Sensory Radius     | $\times (1.0 - 0.3 \times \text{age\_factor})$ |
| Energy Efficiency  | $\times (1.0 - 0.2 \times \text{age\_factor})$ |
| Mortality Hazard   | $h_0 \cdot e^{\beta \cdot \text{age}}$ (Gompertz) |

---

## 4. Starvation Mechanics

Energy is consumed each tick from movement and cognition. When energy depletes:

| Energy Level | State         | Effect                                          |
|-------------|---------------|------------------------------------------------|
| > 20%        | Healthy        | Normal stats                                    |
| 10–20%       | Hungry         | Gather utility × 1.5; Reproduce utility = 0.0  |
| 1–10%        | Starving       | Speed −30%; Cognition simplified                |
| 0%           | Critical       | Health −2.0/tick; entity moving toward death    |

---

## 5. Death & Decay Resource

On death, a `ResourceComponent` node is created at the entity's last position:

```cpp
ResourceComponent decay;
decay.type       = ResourceType::Food;
decay.richness   = entity.mass * DECAY_ENERGY_FACTOR;
decay.maxRichness = decay.richness;
decay.regenRate  = 0.0f;        // Decay nodes do not regenerate
decay.isRenewable = false;
```

This transfers the entity's accumulated biological mass back into the ecosystem, satisfying conservation (Axiom 5). Over time, the decay node is consumed by other shapes, completing the biomass cycle.
