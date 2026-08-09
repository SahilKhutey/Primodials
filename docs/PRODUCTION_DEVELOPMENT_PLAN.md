# Primordials — Production Development Plan
## Game Product & Living Wallpaper Engine — Complete Package

This is the capstone document for the session: every prior finding, fix, and roadmap item,
organized into an actual production plan. Written from all three seats established at the
start — Publisher, Investor, Solo Dev — because a production plan has to satisfy all three at
once or it isn't a real plan.

---

## 0. Executive Summary

**Ship the web-based Wallpaper product first — this remains true.** But this session also
did something new: it actually attempted to build the C++ engine for the first time, rather
than treating its claims as unverified. The result meaningfully changes how "unverified" that
codebase should be considered — worth reading §2a below before treating Product B as vaporware.

**Three candidate products exist in this repo. The verification status of each has changed:**

| Candidate | Tech | Status |
|---|---|---|
| **Primordials — Living Wallpaper** | React/TS/Vite (`src/`) | ✅ **Verified working** — builds, boots, simulates, has a built Wallpaper Mode, now has a dedicated Workshop build variant |
| Polygonal Primordials — full game | C++23 engine (`Engine/`, `Games/`) | 🔶 **Substantially real, not vaporware** — see §2a. Compiles to a working binary after 2 small fixes; genuinely passes its full test suite; the actual game binary boots and runs a live game loop. But ships with real, uncaught bugs and its readiness documentation is provably inaccurate. |
| `PolygonalPrimordialsWallpaper.exe` | C++, referenced in `Tools/master_verify.py` | ⬜ Still doesn't appear to exist — no corresponding source found anywhere in `Engine/`/`Games/`/`Launcher/` |

**Recommendation, updated but not reversed:** still ship the web Wallpaper product first — it
remains the faster, lower-risk path and nothing in §2a changes that. But the full C++ game is
no longer fairly described as "unverified" — it's real, substantial, working engineering that
was simply never actually run through a clean build before its documentation claimed it was
ready to ship. That's a meaningfully different, more fundable situation than "might be
vaporware," and worth knowing if Product B is ever revisited.

---

## 1. Product Strategy — One IP, Staged Products

- **Product A (ship first): Primordials — Living Wallpaper.** A living-ecosystem desktop
  background. Distribution via Wallpaper Engine Workshop (fast, free reach) and/or a standalone
  packaged app later. This is the actual deliverable of this entire session's fixes.
- **Product B (later, optional): Polygonal Primordials — the full game.** The C++ engine's
  8-phase ambition (netcode, Steamworks, LiveOps). Treat as a separate future project that may
  reuse the *concept* and *brand*, not a blocker for Product A.
- **Retire, don't build: the native C++ wallpaper target.** It's referenced by tooling but has
  no source. Building it would mean solving — in C++ — a problem the web app has already
  solved. There's no scenario in a solo-dev, ship-fast plan where this is worth starting.

---

## 2. Current Production State (grounded in this session's actual work)

**Fixed and verified this session:**
- App no longer crashes to a blank screen on a fresh clone with no `.env` (root cause: an
  unconditional `createClient()` call; now conditional, `supabaseEnabled` flag added)
- Cloud-history UI correctly communicates its disabled state instead of failing silently
- Wallpaper Mode no longer silently overwrites user-tuned simulation settings on entry
  (`applyThemeAndPacing` is now only called from explicit theme/pacing selection, verified live
  both directions: custom values survive mode entry; explicit theme picks still apply correctly)

**Confirmed working, not modified:**
- Clean `npm install` / `typecheck` / `build` (1,563 modules, ~470KB JS gzipped to ~132KB)
- Wallpaper Mode UI (`WallpaperDock`, `AmbientHUD`) — genuinely polished: idle-fade chrome,
  theme picker (6+ themes), pacing presets, feature toggles
- **Zero external network dependencies once Supabase is optional** — confirmed via a full
  `grep` for `fetch`/`axios`/`XMLHttpRequest` across `src/`: none exist outside the Supabase
  client. This means the product can ship as a fully offline, static, self-contained build with
  no backend, no hosting cost, no ongoing infrastructure — a meaningful fact for the business
  case in §6.

**Confirmed broken / open (from `test-plan.md`):**
- 1 remaining live UI bug: click-to-inspect is dead in Wallpaper Mode (TC-F3)
- 19 further items across entity visuals, neural/ML, environment, behavior, and atmosphere —
  fully catalogued with reproduction steps in `test-plan.md`, sequenced in
  `development-checklist.md`

---

## 2a. New: The C++ Engine Was Actually Built This Session — Here's What's Real

Every prior pass through this repo treated the C++ engine as unverified and left it that way.
This session went further: installed `cmake`, ran a real `cmake -B build` configure (which
fetches Catch2 and SDL3 from GitHub via `FetchContent` — both succeeded cleanly), then ran the
actual build.

**What was found, in order:**

1. **`SHIP_READINESS.md` claims "No compiler errors or warnings" — false as found.** The build
   hit two real, unrelated compile errors: a missing `#include <cmath>` in
   `Engine/UI/Heatmap/HeatmapToggle.cpp` (undeclared `fabsf`), and an ignored `ftruncate()`
   return value in `Engine/GameView/SharedStateSync.cpp` that fails under `-Werror`. Both are
   the kind of error a single real CI build would catch immediately — their presence is direct
   evidence nobody actually ran this exact build before writing "APPROVED FOR SHIPMENT."
2. **Both fixes were small and correct** (added the missing include; changed the ignored
   `ftruncate` call to check its result and log/abort on failure, matching the file's own
   existing error-handling style for the adjacent `shm_open` call).
3. **After those two fixes, the full build reached 100% with no further errors** — every engine
   module (`ShapeEngine`, `ShapeReplay`, `ShapeNetwork`, `ShapeScripting`, `ShapeCompute`,
   `ShapeAssetPipeline`, `ShapeOptimization`), the test binary, several demos and benchmarks,
   and critically **`PolygonalPrimordials` itself linked successfully as a real 4.2MB
   executable.**
4. **The real test suite (run from the correct working directory) fully passes**: `All tests
   passed (1412 assertions in 221 test cases)`. This differs from `SHIP_READINESS.md`'s stated
   "190 test cases, 1,311 assertions" — the real numbers are higher, suggesting the
   documentation is simply stale rather than fabricated. Either way, the *actual* current test
   suite genuinely passes end to end, once the build errors are fixed. Give real credit here:
   once past the two build bugs, the engineering underneath is substantially solid.
5. **The compiled game binary actually boots and runs.** With no display available in this
   sandbox, it correctly fails at video-device init (expected, environmental, not a bug). Forcing
   `SDL_VIDEODRIVER=offscreen`, the game initialized every subsystem (reproduction, combat,
   speciation, crash reporting, telemetry, achievements, Steam integration correctly falling
   back to stub mode) and **ran its main loop cleanly for a full 20 real seconds** before being
   stopped — about as strong a "this actually runs" signal as is obtainable without a GPU.
6. **A real, separate bug found in the running game**: it logs "Discovered 0 themes from disk"
   even though `Content/Themes/` genuinely contains all 6 themes with valid `theme.json` files.
   Traced to `Engine/Themes/ThemeApplier.cpp` correctly calling
   `themeManager.initialize("Content/Themes")` — but something (likely `ThemeSelector`
   constructing or querying a separate, never-properly-initialized `ThemeManager` instance) means
   the themes never actually reach the selector at runtime. Not fully root-caused; flagged here
   as a genuine, reproducible bug for a future pass, distinct from the two build errors.
7. **A real benchmark ran inside the test suite**, bearing directly on the README's performance
   claims: creating 1,000,000 entities and iterating basic position/velocity physics for 100
   frames averaged ~287-295 FPS. This is genuine, measured evidence for the *narrow* claim it
   covers (bare ECS iteration) — it is not evidence for "1M entities at 60 FPS" in the full game
   sense (with rendering, AI brains, genetics, collision all active), which remains unverified.

**Net assessment, stated fairly:** the C++ codebase is not vaporware and not primarily
AI-scaffolded filler — it's a large, substantially working engineering effort that had never
been run through an actual clean build. The failure mode here isn't "the code doesn't work,"
it's "nobody verified it before documenting that it worked." That's a very different, more
promising starting point for Product B than this document previously assumed — while the
specific claims in `SHIP_READINESS.md`, `launch_checklist.py`, and the Marketing assets (§2b)
remain unreliable and should still be independently re-verified, not trusted, item by item.

## 2b. New: Marketing Assets Do Not Depict the Actual Product

Separately from the engine itself, `Marketing/screenshots/*.jpg` were inspected directly. They
show a fully 3D-rendered game with HP/Mana bars, ability hotkeys, "ZONE" labels, a
"GENOME INSPECTOR" with a DNA-helix visualization, and quest objectives ("Observe Creature
Behavior"). **None of this matches the actual product** — not the verified 2D web app, not
anything the SDL3-based "Shape Engine" (a 2D polygon renderer, per its own name and the
verified rendering code) is capable of producing. These are almost certainly AI-generated
concept art, not real captures, and using them on an actual store listing would be actively
misleading to customers — the kind of mismatch that draws negative reviews the moment real
screenshots or footage circulate. `Marketing/store_page/description.txt` compounds this: it
claims "250,000+ entities," OpenGL 4.6 *and* Vulkan backends, and "a visual node graph editor"
— none independently verified this session, and the entity count doesn't even match the
README's separate claim of 1,000,000. **Do not use any current file in `Marketing/` for a real
listing without rebuilding it from actual verified footage and accurate, consistent claims.**

---

## 3. New Finding: Two Disconnected Production Pipelines, Not Just Two Codebases

Earlier in this session, the finding was "two disconnected codebases" (web app vs. C++ engine).
Inspecting `Tools/` (30+ scripts: `step1_code_health.py` through `step7_launch.py`,
`master_verify.py`, `launch_checklist.py`, `deploy_itch.py`, `steamworks_upload.py`,
`crash_log_analyzer.py`, `post_launch_ops.py`, and more) shows this extends to **production
tooling** as well:

- `master_verify.py` checks for `build/bin/PolygonalPrimordials.exe` and
  `build/bin/PolygonalPrimordialsWallpaper.exe` — both native Windows binaries.
- `deploy_itch.py` expects `release/itch/PolygonalPrimordials-v2.0.0-win.zip`.
- `launch_checklist.py`'s very first check is `("C++ Unit tests pass", True)` — a hardcoded
  boolean, not an actual test invocation. Worth being direct about what this means: this isn't
  a real passing check, it's a placeholder that always reports success. Treat every claim in
  this pipeline as unverified until proven otherwise, the same caution already applied to the
  README's performance claims.

**None of this tooling applies to Product A.** The web Wallpaper product doesn't produce a
`.exe`, doesn't need Steamworks, and doesn't fit an itch.io Windows-zip flow. Building Product
A's actual release pipeline means writing a small, new, much simpler set of scripts — not
adapting these.

---

## 4. Technical Roadmap Reference

Full 22-item findings table and 6-phase sequencing already exist in
`unified-visual-environment-entity-roadmap.md`. Status snapshot as of this document:

| Phase | Status |
|---|---|
| Phase 0 — Close verification gaps | ✅ Done |
| Phase 1 — Correctness fixes | 🔶 2/3 done (chemical field resize still open) |
| Phase 2 — Foundational (spatial index, brain weight preservation, Ecosystem Mood) | ⬜ Open |
| Phase 3 — Entity legibility | ⬜ Open |
| Phase 4 — Environment depth | ⬜ Open |
| Phase 5 — Bigger bets | ⬜ Open |
| Phase 6 — Polish (includes TC-F3, the one remaining live bug) | ⬜ Open |

**Production framing on this roadmap:** none of Phases 2–6 are launch-blocking for Product A.
A living wallpaper that simulates population/species/evolution with the current feature set is
already a complete, sellable product. Phases 2–6 are post-launch quality/depth improvements,
not gates. This distinction matters enormously for a solo dev — it's the difference between
"ship in weeks" and "ship whenever the roadmap is done" (never).

---

## 5. Packaging & Distribution Plan

### 5.1 Primary path: Wallpaper Engine Workshop
Fastest route to real users, zero distribution infrastructure needed. Wallpaper Engine accepts
HTML/JS "web" wallpaper types natively and hosts distribution via Steam Workshop — an existing
audience of millions, no storefront of your own required.

**What's needed, concretely:**
1. ✅ **Done.** A production build variant with zero required Supabase dependency — implemented
   as `npm run build:wallpaper` (`vite build --mode wallpaper`), gated by
   `VITE_WALLPAPER_ONLY=true` in the new `.env.wallpaper` (safe to commit — no secrets).
2. ✅ **Done.** The build now boots directly into Wallpaper Mode (`wallpaperMode` initial state
   reads the flag) with the "Exit Wallpaper" button hidden entirely — verified there's no path
   back to the dense Simulation View in this build. Regular `npm run build` is unaffected and
   still defaults to Simulation View as before — confirmed both directions.
3. ✅ **Researched.** Wallpaper Engine's actual submission process is **not** a manifest you
   hand-author — it's GUI-driven, done entirely inside the (Windows, Steam-distributed)
   Wallpaper Engine editor: drag your built `index.html` folder onto "Create Wallpaper," it
   auto-generates `project.json`, then **Workshop → Share wallpaper on Workshop** publishes it.
   There is no separate manifest-writing step to do in this codebase — confirmed from
   `docs.wallpaperengine.io`'s own "Creating a Web Wallpaper" guide. Two real requirements this
   surfaced that do matter for the build itself, both already satisfied: (a) *all* files must
   load locally, nothing fetched from the web — confirmed true (§2, zero required external
   calls); (b) the wallpaper should scale cleanly across aspect ratios including ultra-wide
   (21:9) — **not yet specifically tested**, worth a manual check before submission.
4. ✅ **Built.** Wallpaper Engine has its own **native settings panel** (shown in its Installed
   tab, separate from anything rendered inside the wallpaper itself), driven by a
   `window.wallpaperPropertyListener.applyUserProperties(...)` callback that Wallpaper Engine
   calls whenever the user changes a property. Implemented `src/lib/wallpaperEngineBridge.ts`,
   installed only in the `WALLPAPER_ONLY` build, bridging that native panel to the *exact same*
   `handleThemeChange`/`handlePacingChange`/`handleToggleSetting` functions already verified
   working. Verified live by simulating Wallpaper Engine's own call shape: a `theme` property
   change correctly drove `maxPopulation` 400→200 (Crystal Cave's real default), and a
   `chemicalField` boolean property correctly toggled that setting true→false. **Remaining
   manual step, cannot be done from this sandbox**: the actual `theme`/`pacing`/boolean
   properties still need to be *created* once inside the Wallpaper Engine editor's "Change
   Project settings" screen (exact key names documented in the bridge file's header comment) —
   that's an interactive GUI step on a Windows machine with Wallpaper Engine installed, not
   something scriptable from here.

### 5.2 Secondary path: standalone packaged app (Tauri)
For later, after Workshop validates demand. Wraps the same build in a native window, pinned
behind desktop icons via OS-level tricks (`WorkerW` reparenting on Windows, etc.). Higher
effort, full control over pricing/distribution — a reasonable Phase 2 business move once
Workshop traction (favorites, comments, subscribe counts) justifies the extra investment.

### 5.3 What NOT to reuse
The existing `Tools/` pipeline (§3) — it's built for a Windows `.exe` + Steamworks + itch.io
flow that doesn't match either distribution path above. A genuinely simple replacement (a
handful of scripts: build, zip, checksum) is a half-day task, not worth adapting 30 scripts
built for a different target.

---

## 6. Business Case (Investor View)

**Revenue reality, stated plainly:** this is a solo-dev side-income product, not a venture-scale
opportunity. The dominant competitive dynamic in this category (confirmed via market research
earlier in this session) is Wallpaper Engine itself — a one-time ~$4 purchase unlocking
unlimited free Workshop content. Realistic expectations: Workshop items are typically free or
low-cost with optional tips/donations; a standalone paid app (Product A, §5.2) in the $3-8 range
is the more realistic revenue vehicle, and even then, modest (hundreds to low thousands of
dollars/month range for a successful niche wallpaper app, not more, absent unusual virality).

**Operating cost model — genuinely good news:** with Supabase now optional and confirmed as the
*only* external dependency in the entire codebase, Product A can run with **zero ongoing
infrastructure cost** — no server, no database, no API bills. This is a rare, clean cost
structure: development time is the only real cost, which suits a solo, self-funded build.

**IP/ownership status:** `LICENSE` is a clean proprietary "all rights reserved" license, full
ownership with Sahil Khutey, no ambiguity, no accidental permissive licensing that would
undercut a commercial sale. No action needed here — it's already correctly set up for a
commercial product.

**Risk register:**
| Risk | Severity | Mitigation |
|---|---|---|
| Bolt.new AI-scaffold origin — **confirmed** via `.bolt/config.json` (`"template": "bolt-vite-react-ts"`) for the web app | Low for Product A (independently verified working regardless of origin) | No longer purely speculative — origin is confirmed. Doesn't change Product A's verified status. |
| C++ engine documentation (`SHIP_READINESS.md`, `launch_checklist.py`) makes false/unverified claims | Medium — actively misleading if trusted at face value | **Now substantiated, not just suspected**: real build hit 2 real compile errors contradicting "no compiler errors." Treat every unverified claim in these docs as needing independent re-verification, not as false — the engine itself proved substantially real once actually built (§2a). |
| Marketing assets (`Marketing/screenshots/*.jpg`, store descriptions) do not depict the real product | **High if ever used for a real listing** | Confirmed directly (§2b) — fully 3D, HP/Mana/combat-HUD imagery for a 2D ALife sim with no such systems visible in code. Must not be used for any real store page without replacement using actual footage. |
| Supabase schema has no per-user isolation if cloud history is ever enabled publicly | Medium, only if Product A ever turns Supabase on for multi-user use | Confirmed via migration file: RLS grants full anon CRUD, no `user_id` column, explicitly "single-tenant, no auth" by design. Fine for solo testing; do not enable for a public multi-user release without adding real auth + ownership. |
| Key-person risk — solo developer, no team redundancy | Standard for this project type | Not really mitigable at this stage; standard solo-dev risk, worth naming rather than ignoring in any future pitch |
| Dual-codebase, dual-pipeline confusion (§3) | Medium — wastes solo-dev time if not resolved | Resolved by this document: Product A is canonical, Product B/C are shelved for now — though §2a means Product B is a more credible future bet than previously assessed |

---

## 7. Solo Dev Execution Plan — Milestone Schedule

Sized for one person, part-time-realistic estimates (not "if I coded 12 hours a day"):

**Milestone 1 — Launch-ready build (1–2 weeks)**
- Fix TC-F3 (dead click-to-inspect) — few hours
- Build the Workshop-specific build variant (§5.1.1, §5.1.2) — 1-2 days
- Research and implement actual Wallpaper Engine manifest requirements (§5.1.3) — 1-2 days,
  mostly reading their docs + trial upload
- Fix chemical field data loss on resize (Phase 1 leftover) — half a day, cheap correctness win
  worth including even though not launch-blocking, since it's this quick

**Milestone 2 — Workshop launch (target: end of Milestone 1 + a few days for upload/review)**
- Submit to Wallpaper Engine Workshop
- Post to r/wallpaperengine, r/desktopfeedback for organic signal (as discussed earlier in this
  session) — free, fast validation before investing further

**Milestone 3 — Iterate on signal (ongoing, weeks 3+)**
- If traction: begin Phase 2 roadmap items (spatial index, Ecosystem Mood) — these compound
  into a visibly richer wallpaper, good candidates for a "v2" Workshop update that re-surfaces
  the listing
- If traction: begin standalone app (§5.2) as a paid product
- If limited traction: this is genuinely useful information cheaply obtained — re-evaluate
  before sinking more time into Phases 3-6

**Explicitly not on this timeline:** the full C++ game, the native wallpaper `.exe` target, and
any of the `Tools/` pipeline. Revisit only if Product A validates and there's specific appetite
to expand scope.

---

## 8. Launch Readiness Checklist (Launch-Blocking Subset)

Distilled from `test-plan.md`/`development-checklist.md` — this is the actual gate for
Milestone 2, not the full 22-item roadmap:

- [x] App boots without crashing on a fresh environment (no `.env`) — TC-A1 ✅
- [x] Clean typecheck/build — TC-A2/TC-A3 ✅
- [x] Wallpaper Mode settings don't silently reset — TC-F2 ✅
- [x] Click-to-inspect doesn't dead-end silently — TC-F3 ✅ (WallpaperInfoPopover)
- [x] Workshop-specific build variant exists (defaults into Wallpaper Mode, no Supabase env
      needed at all, no way to reach dense Simulation View) ✅ `npm run build:wallpaper` —
      verified: boots straight into Wallpaper Mode, "Exit Wallpaper" hidden, no Simulation
      View tabs reachable, zero console errors, no `.env` required.
- [x] Wallpaper Engine submission process researched ✅ — it's GUI-driven (drag `index.html`
      into the Wallpaper Engine editor, publish via its own Workshop menu), not a manifest to
      hand-author. Native settings bridge (`wallpaperEngineBridge.ts`) built and verified so
      Wallpaper Engine's own property panel can drive the wallpaper once properties are created
      in their editor — see production plan §5.1 item 4 for the exact key names to use.
- [ ] Manual step (cannot be done from this sandbox): create the `theme`/`pacing`/toggle
      properties inside the actual Wallpaper Engine editor on a Windows machine, matching the
      key names documented in `src/lib/wallpaperEngineBridge.ts`.
- [ ] Manual step: verify the build scales cleanly on ultra-wide (21:9) aspect ratios —
      Wallpaper Engine's own guidance flags this as a common web-wallpaper failure point, not
      yet specifically tested in this session.
- [ ] Manual smoke test: install the packaged Workshop item on an actual machine running
      Wallpaper Engine, confirm it behaves correctly sitting behind desktop icons, confirm
      auto-pause-on-fullscreen-app behavior (flagged as a correctness requirement back in the
      entity-design doc, not yet implemented — worth confirming Wallpaper Engine's own
      auto-pause handles this at the host level before building a redundant in-app version)

Everything else in `test-plan.md` (Phases 2–6) is **post-launch**, not pre-launch.

---

## 9. Post-Launch Ops (Right-Sized)

The README's Phase H (LiveOps dashboards, Discord moderation bots, cohort analysis) is game-
studio-scale tooling, irrelevant to a solo wallpaper product. Right-sized equivalent:

- **Feedback loop:** Workshop comments + a simple itch.io/Discord presence if traction
  justifies it — not custom dashboards.
- **Update cadence:** ship Phase 2/3 roadmap items as periodic Workshop updates once there's
  a user base to update for — each one is a natural re-engagement moment (Workshop surfaces
  updated items).
- **Crash/error visibility:** given zero backend (§6), there's no telemetry pipeline by
  default — worth deciding explicitly whether that's acceptable (simplest, most private) or
  whether a lightweight opt-in error reporting hook is worth the added complexity. No strong
  recommendation either way; flagging as a decision point, not a gap.

---

## 10. Appendix — Session Artifact Index

Everything produced this session, for reference:

| File | Purpose |
|---|---|
| `entity-design-principles.md` | Trait→visual channel mapping, entity legibility rules |
| `ml-evolution-ai-parameters.md` | Neural brain architecture, 3 concrete bugs found |
| `open-world-environment-interactions.md` | World growth, chemical field, camera, biome findings |
| `neural-behavioral-systems.md` | Brain-vs-instinct architecture split, O(n²) scaling risk |
| `entity-attributes-control-modes.md` | Genome/Organism inventory, control-mode bugs |
| `unified-visual-environment-entity-roadmap.md` | 22-item findings table, 6-phase roadmap, Ecosystem Mood concept |
| `test-plan.md` | 21 concrete test cases, honest pass/fail status |
| `development-checklist.md` | Actionable checklist cross-referenced to test cases |
| `app-tsx-fixes.patch` / `App.tsx` | Verified fixes: Supabase crash guard + settings-clobbering fix |
| `supabase.ts` / `.env.example` | Verified fix: graceful Supabase fallback |
| **This document** | Production plan tying all of the above into a shippable path |

**The one-sentence version of this entire plan:** ship the web Wallpaper product to the
Workshop first — it already works — and treat everything else (the full game, the native
wallpaper target, the deep roadmap phases, the existing production tooling) as explicitly
out of scope until that first, real product has told you whether it's worth the next bet.
