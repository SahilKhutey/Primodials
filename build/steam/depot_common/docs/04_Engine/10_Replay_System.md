# 10_Replay_System

This document specifies the replay capture format, deterministic input logs, and keyframe snapshots.

---

## 1. Replay Stream Recording
Replays do not record full coordinate snapshots every frame (which would be too large). Instead, they store:
* The initial simulation seed and scenario parameters.
* A list of input events tagged by tick index:
  ```cpp
  struct ReplayInputEntry {
      u64        tickIndex;
      InputEvent input;
  };
  ```

---

## 2. Playback Synchronization
During playback:
1. The engine is initialized using the recorded seed.
2. The tick scheduler runs normally, injecting input packets at their recorded tick indices.
3. Because the engine is fully deterministic, this re-simulation reconstructs the exact original state history.

---

## 3. Replay Keyframes (Fast-Seeking)
To support seeking (fast-forwarding or rewinding) in replays, the system creates keyframe snapshots at regular intervals (e.g. every 1,000 ticks):
* **Seeking Forward:** Load the nearest previous keyframe, then run the simulation loop at maximum speed (100,000x) until the target tick is reached.
* **Seeking Backward:** Load the nearest previous keyframe, then re-simulate forward to the target tick.
