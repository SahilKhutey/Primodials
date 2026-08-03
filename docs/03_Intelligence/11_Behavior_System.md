# 11_Behavior_System

This document specifies the behavior system — how selected actions translate into steering forces and collision avoidance vectors.

---

## 1. Kinematic Steering Vectors
Once an action is selected, the entity generates a target steering force vector $\mathbf{F}_{\text{steer}}$:

$$\mathbf{F}_{\text{steer}} = \text{Seek}(\mathbf{p}_{\text{target}}) + \text{AvoidObstacles}()$$

**Seek Behavior:**
$$\text{Seek}(\mathbf{p}_{\text{target}}) = \text{Normalize}(\mathbf{p}_{\text{target}} - \mathbf{p}_{\text{self}}) \times F_{\text{max}}$$

---

## 2. Obstacle Avoidance (Raycasting)
To prevent shapes from colliding with walls or dynamic threats, the behavior layer projects three short raycasts forward:

```
         Avoidance Ray Left
               \
                \     Obstacle
       Heading ──●──────■
                /
               /
         Avoidance Ray Right
```

If a ray intersects an obstacle:
$$\mathbf{F}_{\text{avoid}} = \text{Reflect}(\mathbf{v}, \mathbf{n}_{\text{wall}}) \times F_{\text{avoid\_weight}}$$
This lateral force is added to $\mathbf{F}_{\text{steer}}$, steering the entity smoothly around the collision coordinate.

---

## 3. Kinetic Damping
Entities apply steering thrust directly to their [RigidBodyComponent](file:///C:/Users/ASUS/Documents/Primodilas/Engine/Physics/RigidBodyComponent.hpp):
$$\mathbf{F}_{\text{total}} = \mathbf{F}_{\text{steer}} + \mathbf{F}_{\text{avoid}}$$
Drag and friction damping are then applied by the physics solver to stabilize speeds.
