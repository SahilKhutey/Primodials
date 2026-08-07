# 05_Mathematical_Framework

This document outlines the linear algebra, geometry, and graph algorithms used to calculate spatial coordinates, areas, and utilities.

---

## 1. Vector and Matrix Geometry
* **Vector2 Math:** Position vectors use column-major representation supporting addition, scalar multiplication, dot products, cross-product magnitudes, and Euclidean distance.
* **Transforms:** Vector rotations utilize 2D orientation angles:
  $$\begin{aligned}
  x' &= x \cos(\theta) - y \sin(\theta) \\
  y' &= x \sin(\theta) + y \cos(\theta)
  \end{aligned}$$
* **C++ Symbols:** [Vector2](file:///C:/Users/ASUS/Documents/Primodilas/Engine/Math/Vector2.hpp).

---

## 2. Polygon Properties
* **Shoelace Formula (Gauss's Area):**
  $$\text{Area} = \frac{1}{2} \left| \sum_{i=0}^{n-1} (x_i y_{i+1} - x_{i+1} y_i) \right|$$
* **Centroid (Center of Mass):**
  $$\begin{aligned}
  C_x &= \frac{1}{6 A} \sum_{i=0}^{n-1} (x_i + x_{i+1}) (x_i y_{i+1} - x_{i+1} y_i) \\
  C_y &= \frac{1}{6 A} \sum_{i=0}^{n-1} (y_i + y_{i+1}) (x_i y_{i+1} - x_{i+1} y_i)
  \end{aligned}$$
* **Moment of Inertia ($I$):**
  $$I = \frac{M}{6} \frac{\sum \left( |x_i y_{i+1} - x_{i+1} y_i| (r_i^2 + r_i \cdot r_{i+1} + r_{i+1}^2) \right)}{\sum |x_i y_{i+1} - x_{i+1} y_i|}$$
* **C++ Symbol:** [PolygonGeometry](file:///C:/Users/ASUS/Documents/Primodilas/Engine/Math/PolygonGeometry.hpp).

---

## 3. Probability & Deterministic Randomness
* **Random Seeds:** All stochastic processes use a `SimRNG` seeded from the world seed. See [18_Deterministic_Simulation](file:///C:/Users/ASUS/Documents/Primodilas/docs/01_Foundations/18_Deterministic_Simulation.md).
