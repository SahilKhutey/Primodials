# 10_Networking_Framework

This document specifies the Networking Framework.

---

## 1. Core Specifications

* **Purpose:** Handles deterministic lockstep packet transmission, desync identification, and input rollback synchronization.
* **Responsibilities:** packing player actions, buffering input packets, checking desync hashes, fast-forwarding simulation ticks.
* **Interfaces:** `INetworkConnection`, `IRollbackSolver`.
* **Managers:** `NetworkManager`.
* **Systems:** None (intercepts simulation tick entrances).
* **Components:** None.
* **Events:** `DesyncDetectedEvent` (emitted on CRC32 hash mismatch).
* **Resources:** Target IP packet queues.
* **Algorithms:** Rollback re-simulation fast-seeking, LCG state serialization checks.
* **Threading:** Runs on a dedicated network socket thread, writing input queues atomically.
* **Serialization:** Compact binary packing of user input command streams.
* **Testing:** Validation fuzzer mocks packet loss, asserting successful rollback re-simulations.
* **Debugging:** Displays connection latency timeline bars and packet drop logs.
