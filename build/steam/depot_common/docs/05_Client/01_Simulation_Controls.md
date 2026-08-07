# Simulation Controls & Player Interaction

## Overview

The Primodilas engine exposes game-like interactive controls so users can observe, manipulate, and explore simulations in real time. Three cooperating systems handle this layer:

| System | File | Purpose |
|---|---|---|
| `SimulationController` | `Engine/Simulation/SimulationController.h` | Playback state machine |
| `CameraController` | `Engine/Rendering/CameraController.h` | 2D viewport navigation |
| `GameInputController` | `Engine/Rendering/GameInputController.h` | Input action mapping |

---

## SimulationController

The simulation controller manages a formal state machine over the full lifecycle of a simulation session.

### States

```
       ┌──────┐
 ──▶   │Stopped│ ◀─────────────────────────┐
       └──┬───┘                            │
          │ Play()                          │ Stop()
          ▼                                │
       ┌─────────┐   Pause()   ┌────────┐  │
       │ Running │────────────▶│ Paused │──┘
       └────┬────┘             └───┬────┘
            │ FastForward()        │ Play()
            ▼                     │
       ┌──────────────┐           │
       │ FastForward  │◀──────────┘
       └──────────────┘
            │ StepSingleTick()
            ▼
       ┌──────────────────┐
       │ StepSingleTick   │ (auto-returns to Paused)
       └──────────────────┘
```

### Speed Presets

```cpp
sim_ctrl.SetSpeedPreset(0); // 0.25× — slow-motion
sim_ctrl.SetSpeedPreset(1); // 0.5×
sim_ctrl.SetSpeedPreset(2); // 1.0× — real-time (default)
sim_ctrl.SetSpeedPreset(3); // 2×
sim_ctrl.SetSpeedPreset(4); // 5×
sim_ctrl.SetSpeedPreset(5); // 10× — fast-forward
```

### Key API

```cpp
sim_ctrl.Play();
sim_ctrl.Pause();
sim_ctrl.Stop();
sim_ctrl.TogglePause();
sim_ctrl.StepSingleTick();   // Advance exactly 1 tick then return to Paused
sim_ctrl.FastForward();

bool running  = sim_ctrl.IsRunning();
bool paused   = sim_ctrl.IsPaused();
float scale   = sim_ctrl.GetTimeScale();
uint64_t tick = sim_ctrl.GetCurrentTick();
```

---

## CameraController

Manages a 2D viewport camera for navigating the simulation world.

### Features

| Feature | Description |
|---|---|
| **Pan** | Move camera position in world space |
| **Zoom** | Scale factor with configurable min/max clamp |
| **Entity Follow** | Smooth lerp tracking to a target entity's `TransformComponent` |
| **WorldToScreen** | Projects world coordinates to screen pixel position |
| **ScreenToWorld** | Unprojects screen pixel position to world coordinates |

### Coordinate System

```
Screen (0,0) ─────────────────▶ Screen (W, 0)
    │                                   │
    │      Viewport                     │
    │   camera.pos = world origin       │
    │                                   │
Screen (0,H) ──────────────── Screen (W,H)
```

```cpp
// WorldToScreen
Vector2f screen = camera.WorldToScreen(world_pos, screen_size);

// ScreenToWorld
Vector2f world = camera.ScreenToWorld(mouse_pos, screen_size);

// Entity tracking
camera.FollowEntity(entity_id);
camera.SetFollowLerpSpeed(5.0f);  // Higher = snappier

// Zoom
camera.SetZoom(2.0f);    // 2× zoom
camera.ZoomIn(0.1f);
camera.ZoomOut(0.1f);
```

---

## GameInputController

Maps discrete `InputAction` events to `CameraController` and `SimulationController` operations.

### InputAction Enum

```cpp
enum class InputAction {
    TogglePause,
    StepSingleTick,
    ToggleFastForward,
    ZoomIn,
    ZoomOut,
    PanUp, PanDown, PanLeft, PanRight,
    SelectEntity,
    ResetCamera
};
```

### Executing Actions

```cpp
input_ctrl.ExecuteAction(InputAction::TogglePause);
input_ctrl.ExecuteAction(InputAction::ZoomIn);
input_ctrl.ExecuteAction(InputAction::StepSingleTick);
```

The controller bridges raw keyboard/mouse event codes to `InputAction` via a configurable `KeyBinding` map, supporting full remapping.

---

## Test Coverage

| Test File | Cases |
|---|---|
| `EngineTests/PhaseETests/test_simulation_controller.cpp` | All state transitions, speed presets, step-tick increment |
| `EngineTests/PhaseETests/test_camera_controller.cpp` | Pan, zoom clamping, WorldToScreen round-trip, entity follow lerp |
| `EngineTests/PhaseETests/test_end_to_end_user_scenarios.cpp` | Full integrated user scenario: camera follow + playback controls |
