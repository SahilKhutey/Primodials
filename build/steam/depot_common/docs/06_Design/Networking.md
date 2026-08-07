# Networking

This document specifies lockstep packet synchronization, rollback re-simulation, and desync checks.

---

## 1. Input Latency Lockstep
The networking engine enforces a lockstep synchronization model:
* Clients do not transmit coordinate positions or velocities.
* Clients only transmit player inputs (actions, clicks).
* A simulation tick executes only when input packets from all peers for that tick index are received, ensuring identical simulation states on all clients.

---

## 2. Rollback & Resimulation
* If a packet is dropped, the client temporarily halts.
* Once the packet arrives, the client rolls back to the last synchronized state.
* The client fast-forwards by re-simulating the missing ticks to catch up to the server frame.

---

## 3. Desync Checks (CRC32 Hashes)
To verify synchronization, clients calculate and verify state hashes every N ticks:

$$H_t = \text{CRC32}(\text{EntitiesCount} \| \text{Sum(Positions)} \| \text{RNGState})$$

A hash mismatch indicates a desync, halting the simulation and triggering resynchronization routines.
