# 05_Input_Handling_System

This document specifies the input mappings, coordinate raycast projections, and event injection pipeline.

---

## 1. Input Mapping Definitions
The platform wrapper maps user inputs to discrete commands:

| Device Input         | Action Command                      |
|----------------------|-------------------------------------|
| Left Mouse Click     | Inspect shape / select depot        |
| Right Mouse Click    | Place resource magnet / threat      |
| Mouse Scroll / Wheel | Adjust Camera Zoom                  |
| W, A, S, D Keys      | Pan Camera (Up, Left, Down, Right)  |
| Space Bar            | Toggle simulation Pause             |

---

## 2. Coordinate Unprojection
Screen coordinates clicked by the user $(x_s, y_s)$ are converted back to world-space coordinates $(x_w, y_w)$:

$$\begin{aligned}
x_w &= \frac{x_s - \text{Width}/2}{\text{Zoom}} + x_{\text{cam}} \\
y_w &= \frac{y_s - \text{Height}/2}{\text{Zoom}} + y_{\text{cam}}
\end{aligned}$$

---

## 3. Injecting Input Events
* Calculated world coordinates are packed into an `InputEvent` struct.
* The event is pushed to the central `EventQueue` to be processed synchronously on the next tick, ensuring deterministic state progression.
