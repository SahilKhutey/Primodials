# 17_Physics_System

This document provides the complete specification of the Physics System — the module responsible for integrating motion, applying forces, and advancing all kinematic state.

---

## 1. Physics System Responsibilities

```
PhysicsSystem::Update(World& world, f32 dt)
  │
  ├── For each entity with RigidBodyComponent:
  │     ├── Accumulate forces (steering, wind, gravity-analogue)
  │     ├── Integrate velocity: v += (F/M) × dt     [Euler-Cromer]
  │     ├── Clamp velocity to max speed
  │     ├── Integrate position: x += v × dt
  │     ├── Integrate angular velocity: ω += (τ/I) × dt
  │     ├── Integrate angle: θ += ω × dt
  │     ├── Apply linear damping: v *= (1 - drag)
  │     └── Apply angular damping: ω *= (1 - drag)
  │
  └── Rebuild SpatialHashGrid with updated positions
```

**C++ Symbol:** [PhysicsSystem.hpp](file:///C:/Users/ASUS/Documents/Primodilas/Engine/Physics/PhysicsSystem.hpp)

---

## 2. Force Accumulator

Forces are accumulated into each `RigidBodyComponent` before integration:

```cpp
struct RigidBodyComponent {
    Vector2 forceAccum;   // Cleared after each tick
    f32     torqueAccum;  // Cleared after each tick
    // ... position, velocity, mass, etc.
};
```

Systems submit forces before `PhysicsSystem` runs:

| Source             | Force Type             | Submission Point           |
|-------------------|------------------------|----------------------------|
| AI Steering        | Directional thrust     | OptimizationSystem::Update |
| Collision Impulse  | Impulsive (immediate)  | During broadphase/narrow   |
| Wind (Weather)     | Environmental field    | WeatherSystem::Update      |
| Influence Fields   | Push/Pull attractor    | InfluenceSystem::Update    |

---

## 3. Speed Limiting

Maximum speed is derived from genome and mass:
$$v_{\text{max}} = \frac{G_{\text{speed}} \times F_{\text{max}}}{M}$$

After velocity integration, velocity is clamped:
$$\mathbf{v} \leftarrow \hat{\mathbf{v}} \times \min(|\mathbf{v}|, v_{\text{max}})$$

Larger, heavier entities are naturally slower — no special code path required.

---

## 4. Steering Force Calculation

Steering forces are computed from the AI target position:
$$\mathbf{F}_{\text{steer}} = \hat{\mathbf{d}} \times F_{\text{thrust}} \times G_{\text{speed}}$$
where $\mathbf{d} = \text{targetPos} - \text{currentPos}$, and $F_{\text{thrust}}$ is a base constant.

**Arrival Behavior:** Force magnitude scales down as entity approaches the target within radius $R_{\text{arrive}}$:
$$|\mathbf{F}_{\text{steer}}| \leftarrow |\mathbf{F}_{\text{steer}}| \times \min\left(1.0, \frac{|\mathbf{d}|}{R_{\text{arrive}}}\right)$$

This prevents overshoot oscillation around resource nodes.

---

## 5. Energy Cost of Physics

Every tick that an entity applies a non-zero steering force, metabolic energy is consumed:
$$\Delta E_m = -C_{\text{move}} \times |\mathbf{v}|^2 \times M \times \Delta t$$

This enforces the thermodynamic principle that motion has a cost, creating natural selection pressure against wasteful movement patterns.
