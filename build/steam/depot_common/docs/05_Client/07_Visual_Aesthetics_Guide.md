# 07_Visual_Aesthetics_Guide

This document specifies the HSL color palettes, emotional visual shifts, and vector glow aesthetic guidelines.

---

## 1. Curated Color Palette
Shapes are rendered with vibrant HSL color values to represent their core state:
* **Background Grid:** Deep charcoal gray background ($H=220, S=10\%, L=10\%$) with soft grid lines.
* **Sentinel Nodes:** Bright neon cyan ($H=180, S=100\%, L=50\%$) with a pulsing outer ring.
* **Resource Nodes:** Glowing emerald green ($H=120, S=90\%, L=45\%$).

---

## 2. Emotional Color Shifts
Living shapes shift colors dynamically based on their current needs and emotions:

$$\text{Shape Color} = \text{Lerp}(\text{BaseColor}, \text{AlertColor}, \text{Scale})$$

* **High Hunger:** Shifts fill towards pale orange ($H=35, S=80\%, L=40\%$).
* **High Fear:** Shifts fill towards deep warning purple ($H=280, S=100\%, L=30\%$).
* **High Rage:** Shifts border glow towards vibrant crimson red ($H=0, S=100\%, L=50\%$).

---

## 3. Collision Particle Sparks
On high-velocity collisions:
* The client spawns a burst of small, temporary particles at the contact coordinates.
* Particle colors match the colliding shapes, fading in transparency ($L$) over $0.25$ seconds to simulate energy dissipation.
