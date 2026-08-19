# ✅ PRIMODIALS — Biome Diagnostic Validation Rules

The editor executes real-time validation across 3 strict severity tiers: **Errors** (blocks export), **Warnings** (safety alerts), and **Info** (best practice recommendations).

---

## 🔴 ERRORS (Export Blocking)

| Code | Trigger Condition | Rationale |
| :--- | :--- | :--- |
| **`E001`** | Missing required root schema keys (`id`, `species`, `world`, `rules`, `visuals`) | Schema non-conformance |
| **`E002`** | Biome `id` contains non-kebab-case characters | URL / CLI filename incompatibility |
| **`E005`** | Biome contains 0 configured species | Empty simulation |
| **`E006`** | Biome lacks at least 1 `producer` species | Starvation deadlock on tick 0 |
| **`E102`** | Duplicate species `id` identified | Ambiguous entity indexing |
| **`E106`** | Malformed hex color code (e.g. non-`#RRGGBB` format) | Canvas rasterization failure |
| **`E111`** | Species `diet` array contains non-existent species ID | Broken trophic graph dependency |
| **`E112`** | Species lists its own ID in its `diet` array | Self-predation loop |
| **`E113`** | Predator or Grazer configured with 0 food items in `diet` | Guaranteed rapid extinction |
| **`E114`** | Producer configured with dietary requirements | Biological invalidity |
| **`E202`** | `carryingCapacity` is less than sum of initial species counts | Immediate overcrowding death |

---

## ⚠️ WARNINGS (Advisory Alerts)

| Code | Trigger Condition | Recommendation |
| :--- | :--- | :--- |
| **`W001`** | Initial total population exceeds $5,000$ entities | May reduce framerates on low-end mobile devices |
| **`W101`** | Color token is not part of the primary Gensouls brand palette | Suggest adhering to official aesthetic guidelines |
| **`W204`** | Reproduction rate exceeds $0.10$ / second | Risk of exponential population explosions |
| **`W206`** | Species lifespan is less than $30$ seconds | High generational turnover rate |
| **`W301`** | Biome description is under 20 characters | Expand description for community discovery |
