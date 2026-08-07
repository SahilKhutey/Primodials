# 08_Networking_Architecture

This document specifies the lockstep networking protocol, desync detection hashes, and input packet formats.

---

## 1. Input Latency Lockstep
The engine utilizes a lockstep synchronization model:
* Clients do not transmit entity state coordinates (positions, velocities).
* Clients only transmit player inputs (coordinates clicks, seed parameters).
* A tick executes only when input packets from all clients for that tick have arrived, ensuring identical simulation results across clients.

---

## 2. Desync Detection Hashes
To verify that clients remain synchronized, each tick generates a verification hash:
$$H_t = \text{CRC32}(\text{ActiveEntitiesCount} \| \text{Sum(Positions)} \| \text{SimRNGState})$$
At designated intervals, clients exchange and verify these hashes. A hash mismatch indicates a desync, halting the simulation and triggering resynchronization routines.

---

## 3. Resimulation Protocol
When a client experiences temporary network packet drops:
1. The client falls behind the server's tick index.
2. Once the missing input packets arrive, the client rolls back to the last synchronized state.
3. The engine fast-forwards by re-simulating the missing ticks to catch up to the current server frame.
