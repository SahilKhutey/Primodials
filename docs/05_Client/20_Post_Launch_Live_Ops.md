# 20_Post_Launch_Live_Ops

This document specifies the telemetry collectors, error reports, and hotfix patch pipelines.

---

## 1. Diagnostics Telemetry
To analyze player strategies and balance issues:
* **Ecosystem Logs:** The client registers anonymous metrics (e.g. maximum population reached, average shape lifespans, preferred Sentinel policies).
* **Desync Reports:** Any desync events during multiplayer runs trigger a stack dump, uploading LCG seed parameters to the development logs server.

---

## 2. Dynamic Settings Tweaks
To balance the game without compiling new binaries, balance modifiers are loaded from remote parameters:
* Metabolic cost constants.
* Base technology costs.
* Influence field emission radii.

---

## 3. Hotfix Release Pipeline
* Patches compile using static version checks.
* Modifying component schemas triggers automatic version increment alerts during save parsing, protecting user save file libraries.
