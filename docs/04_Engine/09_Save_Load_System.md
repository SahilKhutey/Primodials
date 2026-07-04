# 09_Save_Load_System

This document specifies the serialization layouts, file schemas, and snapshot format guidelines.

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

## 2. Versioning & Backward Compatibility
To prevent issues when loading older save files:
* Every component structure has a unique, hardcoded schema identifier (e.g. `RigidBodyComponent` v1).
* If a component's memory layout changes, a migration function translates older formats to the new schema during loading.

---

## 3. Endianness Safety
To ensure save files are cross-compatible across architectures, all multi-byte integers are serialized in a defined byte order:
* The serialization layer converts values to little-endian format on write.
* The deserialization layer maps values back to the host system's native representation on read.
