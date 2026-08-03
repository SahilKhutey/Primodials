# 06_Audio_System_Design

This document specifies the spatial audio layout, event-cued sounds, and seasonal ambient mixers.

---

## 1. Spatial Audio Volume Scaling
Audio volume decays based on distance from the camera focus point using an inverse distance model:

$$\text{Volume} = \text{BaseVolume} \times \frac{1.0}{1.0 + d}$$

where $d$ is the distance between the sound emitter and the camera center.

---

## 2. Event-Cued Sounds
The audio layer scans the `EventQueue` at the end of each tick to trigger sound effects:
* **CollisionEvent (impulse > threshold):** Plays a metallic impact sound, pitch-scaled by the masses of the colliding shapes.
* **DeathEvent:** Plays a soft structural disintegration tone.
* **ReproductionEvent:** Plays a split tone.

---

## 3. Seasonal Ambient Mixers
Ambient loops fade dynamically based on global season parameters:
* **Summer:** Fades in low-frequency warmth hums.
* **Winter:** Blends in high-frequency wind chime tones.
* **Alert Levels (Red):** Fades in urgent warning hums, shifting the mood as threat alarms activate.
