# 🎨 PRIMODIALS — Biome Editor UI & UX Design Specification

---

## 🖼️ WORKSPACE LAYOUT

```
┌────────────────────────────────────────────────────────────────────────┐
│ [LOGO] Biome Editor    [Visual Mode] [Live Preview] [Raw JSON] [Export]│
├───────────────┬────────────────────────────────────────┬───────────────┤
│ SPECIES LIST  │                                        │ PARAMETERS    │
│               │             LIVE PREVIEW               │ [Species Tab] │
│ + Add Species │         (60 FPS Simulation)            │ [World Tab]   │
│               │                                        │ [Rules Tab]   │
│ ● Algae (400) │                                        │ [Visuals Tab] │
│ ▲ Daphnia(150)│                                        │               │
│ ◆ Copepod(12) │                                        │ Sliders &     │
│               │                                        │ Color Pickers │
├───────────────┴────────────────────────────────────────┴───────────────┤
│ [Status] 60 FPS | 562 Organisms | ✅ Validation: 0 Errors, 1 Warning    │
└────────────────────────────────────────────────────────────────────────┘
```

---

## 🎛️ PANEL SPECIFICATIONS

### 1. Header Toolbar
- **Inline Title Editor:** Immediate inline renaming with auto-kebab-case `id` synchronization.
- **View Selectors:** Three interchangeable view modes (`Visual`, `Live Preview`, `Raw JSON`).
- **Export Action Hub:** Direct file download, clipboard payload copying, and automated GitHub PR template generation.

### 2. Left Sidebar (Species Roster)
- Drag-and-drop hierarchy ordering.
- Trophic status indicator badges (`Producer`, `Grazer`, `Predator`, `Scavenger`).
- Quick-clone and deletion context menus.

### 3. Center Canvas (Live Viewport)
- Real-time simulation loop responding immediately to slider adjustments.
- Overlay stats HUD: framerate counter, live trophic breakdown, and tick timestamp.

### 4. Right Inspector (Multi-Tab Controls)
- **Species Inspector:** Movement velocity, vision radius, metabolic drain, hunt success, and reproduction limits.
- **World Inspector:** Bounds ($W \times H$), toroidal boundary wrapping toggles, and temperature settings.
- **Rules Inspector:** Maximum carrying capacity, extinction floor thresholds, and periodic seasonal climate events.
- **Visuals Inspector:** Background ambient gradients, particle density emitters, and post-processing bloom.
