# 🎯 Polygonal Primordials — Product Definition (v1 FROZEN)

**Status:** LOCKED
**Effective:** 2026
**Author:** Sahil Khutey, Gensouls Lab
**Distribution Phase:** Stage 1 (Validation)

---

## 1. PRODUCT NAME

**Primary:** Polygonal Primordials
**GitHub repo:** polygonal-primordials (current: Primodials — see repo cleanup phase)

---

## 2. ONE-LINE PITCH

> A living ecosystem on your desktop.

---

## 3. ONE-SENTENCE PITCH

Watch a tiny artificial ecosystem evolve, compete, reproduce and
change while you work, study or play.

---

## 4. TARGET AUDIENCE (v1)

**Primary:** Wallpaper Engine users (30M+ active users)
**Secondary:** Cozy / ambient / sandbox simulation fans
**Tertiary:** C++ / simulation technical enthusiasts

---

## 5. PLATFORMS (v1)

- **Wallpaper Engine Workshop** (PRIMARY — free, validates demand)
- **itch.io** (downloadable web build, PWYW)
- **Standalone Windows executable** (TBD, after validation)

**Not in v1:**
- macOS native (later)
- Linux native (later)
- Steam (later, after validation)
- Native C++ game (later, if validated)

---

## 6. CORE GAMEPLAY LOOP (v1)

INSTALL ↓ LAUNCH (default wallpaper mode) ↓ SEE LIVING ECOSYSTEM ↓ WATCH ORGANISMS INTERACT ↓ OBSERVE POPULATION CHANGES ↓ LEAVE IT RUNNING (background) ↓ COME BACK LATER ↓ SEE EVOLVED ECOSYSTEM

---

## 7. V1 FEATURE SET (FROZEN)

### Required (launch blockers)
- ✅ Living artificial ecosystem (predator + prey + producer)
- ✅ Creature movement (continuous, smooth)
- ✅ Reproduction (asexual + sexual depending on species)
- ✅ Death (age, hunger, predation)
- ✅ Population dynamics (Lotka-Volterra-like math)
- ✅ 6 default biomes (algae-bloom, deep-sea, primordial-soup, tundra, coral-reef, volcanic-vent)
- ✅ Interactive camera (zoom, pan)
- ✅ Pause / resume
- ✅ Speed controls (0.25x, 1x, 2x, 4x)
- ✅ Settings panel (entity count, performance mode)
- ✅ Local persistence (save current state)
- ✅ Wallpaper mode (browser fullscreen, transparent overlay)
- ✅ 60 FPS target on mid-range hardware
- ✅ Works offline (no required server)

### Nice to have (post-v1.0)
- Population graphs
- Species inspection
- Genome inspector
- Seed sharing
- Time progression
- Theme selection
- Cloud save (optional)

### Explicitly OUT OF SCOPE for v1
- ❌ Multiplayer
- ❌ Steam Workshop distribution
- ❌ Civilization simulation
- ❌ Complex economy
- ❌ Procedural mega-world
- ❌ Networking architecture
- ❌ Large-scale modding
- ❌ Steam achievements
- ❌ Native C++ standalone
- ❌ Full 3D rendering
- ❌ Mobile version

---

## 8. TECHNICAL FOUNDATION (v1)

| Layer | Technology |
|-------|-----------|
| UI | React 18 + TypeScript |
| Build | Vite |
| Styling | Tailwind CSS |
| Engine | Custom ECS in TypeScript (port of C++) |
| Rendering | Canvas 2D (WebGPU fallback later) |
| State | Zustand |
| Persistence | LocalStorage |

**Note:** The native C++ engine exists in `Engine/` but is **frozen**.
v1 ships with the TypeScript web app (`src/`), which is also the
**primary active product** per the current repository state.

---

## 9. PRICING (v1)

- **Wallpaper Engine Workshop:** FREE
- **itch.io:** PWYW (suggested $5, minimum $0)
- **Standalone Windows:** TBD (after validation)

---

## 10. SUCCESS METRICS (v1, 30 days post-launch)

| Metric | Target | Stretch |
|--------|--------|---------|
| Wallpaper Engine subscriptions | 500 | 2,000 |
| Workshop favorites | 200 | 800 |
| itch.io downloads | 1,000 | 5,000 |
| Comments / feedback | 50 | 200 |
| 7-day retention | 30% | 50% |
| Reddit / TikTok engagement | 10K impressions | 50K impressions |

---

## 11. v1 EXPLICITLY DOES NOT INCLUDE

- ❌ Steam launch
- ❌ Paid commercial version
- ❌ Native binary distribution
- ❌ Native C++ engine activation
- ❌ 250K+ entities (this is an aspirational claim — not v1)
- ❌ Vulkan / OpenGL 4.6 specific rendering
- ❌ 11-language localization
- ❌ 100K+ entity performance on commodity hardware

Any current README, store listing, or marketing material claiming
these features must be **updated or removed**.

---

## 12. v1 EXPLICITLY INCLUDES (verified claims only)

- ✅ Real-time ecosystem simulation
- ✅ Predator-prey dynamics
- ✅ Multiple biomes (6)
- ✅ Desktop wallpaper mode
- ✅ Open source (subject to LICENSE decision — see 01_LICENSE_DECISION.md)
- ✅ Works offline
- ✅ Free / pay-what-you-want
- ✅ Custom biome authoring (via JSON)
- ✅ Community contributions welcome

---

## 13. REVIEW PROCESS

This Product Definition will be reviewed:
- Before v1 launch
- After first 1,000 users
- Quarterly
- Any time v1 scope might change

**Changes require written update to this document.**

---

*"A quiet rebellion against static screens."* — Sahil Khutey
