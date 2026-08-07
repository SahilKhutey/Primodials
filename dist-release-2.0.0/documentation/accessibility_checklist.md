# ♿ Accessibility (A11y) Verification & Checklist — Polygonal Primordials

## Overview
Polygonal Primordials provides accessible UI navigation for players with motor disabilities, low vision, or keyboard-only interaction preference.

---

## 📋 Accessibility Requirements & Verification Matrix

### 1. Keyboard Navigation (Tab / Shift+Tab)
- [x] **Tab Order**: All interactive controls (buttons, text boxes, sliders, cards) have a deterministic tab order managed by `KeyboardNavigator`.
- [x] **Shift+Tab**: Cycles focus in reverse order across active modal overlays.
- [x] **Focus Ring**: Focused elements render a high-visibility pulsing highlight border (`rgba(100, 180, 255, 150-255)`).
- [x] **Activation Keys**: `SPACE` and `ENTER` activate the currently focused element.
- [x] **Escape Cancel**: `ESC` cancels active dialogs, dismisses popups, or clears element focus.

### 2. Low-Vision & Contrast (WCAG AA Compliance)
- [x] **Color Contrast**: All primary text elements maintain a minimum 4.5:1 luminance contrast ratio against dark panel backgrounds (`rgba(15, 18, 25, 245)`).
- [x] **Font Scalability**: UI debug and vector font sizes scale relative to display resolution.
- [x] **Deuteranopia / Protanopia Friendliness**: Trait indicators and heatmap modes avoid red/green-only status distinction by using distinct icons (`✓`, `⚠`, `✕`) and shape geometries.

### 3. Screen Motion & Photosensitivity
- [x] **Smart Motion Reduction**: High-speed particle density and glow intensity are adjustable via `SettingsWindow -> Visual`.
- [x] **Heatmap Toggles**: Heatmap overlay toggle (`H`) allows non-flashing static visualization of ecosystem density.

### 4. Audio Controls
- [x] **Mute on Focus Loss**: Mutes background audio automatically when window loses OS focus.
- [x] **Master Volume Control**: Independent master volume sliders accessible from `SettingsWindow -> Audio`.
