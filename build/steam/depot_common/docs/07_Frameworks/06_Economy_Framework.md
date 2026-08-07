# 06_Economy_Framework

This document specifies the Economy Framework.

---

## 1. Core Specifications

* **Purpose:** Supply-demand pricing calculations and carrier trade loops management.
* **Responsibilities:** Adjusting prices based on storage depots shortages, evaluating trade profit margins, dispatching transport tasks.
* **Interfaces:** `IMarketPricing`, `ILogisticsRouter`.
* **Managers:** `EconomyManager`.
* **Systems:** `LogisticsSystem` (coordinates transport careers).
* **Components:** `StorageComponent`, `CareerComponent`.
* **Events:** `TradeCompletedEvent` (emitted on cargo deliveries).
* **Resources:** Base resource valuations, transportation costs.
* **Algorithms:** Exponential supply-demand pricing curves.
* **Threading:** Query operations are distributed, while pricing updates run on the main thread to ensure synchrony.
* **Serialization:** Saves current depot pricing scales.
* **Testing:** Asserts that cargo is exported when local depots hit maximum storage capacity.
* **Debugging:** Displays active profit margin values and pricing tables over storage buildings.
