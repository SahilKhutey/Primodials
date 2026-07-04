# Serialization

This document specifies the binary schema layout, save snapshot formats, and replay input log structures.

---

## 1. Binary Schema Layout
Save files use a structured binary format to minimize file size and load times:

```
[Header: magic bytes, version] ──→ [Clock State] ──→ [Entities Index] ──→ [Component Arrays]
```

* **Header:** Identifies the file format version and engine build hash.
* **Clock State:** Stores the tick counter and `SimRNG` state.
* **Entities Index:** Lists all active and recycled entity IDs.
* **Component Arrays:** Raw binary dumps of contiguous component dense arrays.

---

## 2. Replay Input Log Format
Replay files log the initial seed and client inputs indexed by tick count:

```cpp
struct ReplayInputEntry {
    u64        tickIndex;
    InputEvent input;
};
```

Playback reads the seed, initializes the world, and re-simulates the ticks by injecting input events at their recorded indexes.
