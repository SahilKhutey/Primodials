# 04_Camera_System

This document specifies the camera zoom and pan parameters, smooth target tracking, and region clamping bounds.

---

## 1. Camera Transform Matrix
The client camera maps world coordinates $(x_w, y_w)$ to screen coordinates $(x_s, y_s)$ using scale (zoom) and translation (pan):

$$\begin{aligned}
x_s &= (x_w - x_{\text{cam}}) \times \text{Zoom} + \frac{\text{Width}}{2} \\
y_s &= (y_w - y_{\text{cam}}) \times \text{Zoom} + \frac{\text{Height}}{2}
\end{aligned}$$

---

## 2. Smooth Target Tracking (Lerp)
When the player selects an entity to follow, the camera updates its focus position smoothly using linear interpolation (Lerp):

$$\mathbf{p}_{\text{cam}, t+1} = \mathbf{p}_{\text{cam}, t} + (\mathbf{p}_{\text{target}} - \mathbf{p}_{\text{cam}, t}) \times K_{\text{lerp}}$$

* **Tracking Weight ($K_{\text{lerp}}$):** $0.1$ per frame (provides a smooth slide-into-focus effect).

---

## 3. Boundary Clamping
To prevent the camera from panning infinitely into empty space, pan offsets are clamped to the bounding boxes of active regions:

$$x_{\text{cam}} = \text{clamp}(x_{\text{cam}}, x_{\text{min\_region}}, x_{\text{max\_region}})$$
$$y_{\text{cam}} = \text{clamp}(y_{\text{cam}}, y_{\text{min\_region}}, y_{\text{max\_region}})$$
Bounds expand dynamically as explorers spawn new regions.
