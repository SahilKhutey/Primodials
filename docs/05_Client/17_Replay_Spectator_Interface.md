# 17_Replay_Spectator_Interface

This document specifies the spectator HUD widgets, timeline scrubbers, and seek operations.

---

## 1. Timeline Controls
When loading a replay, the client renders a timeline control bar at the bottom:

```
[<< Rewind]  [|| Pause]  [> Play]  [>> 2x/4x]  [Next Keyframe >|]
 [●─────────────────────────────────────────────────────────────]  ← Timeline Scrubber
 0                                                          100%  Playback Progress
```

* **Timeline Scrubber:** Dragging the scrubber triggers keyframe seek operations (see [10_Replay_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/04_Engine/10_Replay_System.md)).

---

## 2. Statistical Charts overlay
Renders dynamic history plots (e.g. population curves, resource reserves, death rates) synced with the playback scrubber, giving spectators a clear overview of civilization history.

---

## 3. Free Cam Spectating
During playback, the camera can decouple from tracking specific shapes, allowing the player to pan and zoom freely across the 2D plane to analyze different sectors.
