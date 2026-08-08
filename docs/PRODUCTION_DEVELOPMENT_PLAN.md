# Primordials — Production Development Plan
## Game Product & Living Wallpaper Engine — Complete Package

This is the capstone document for the session: every prior finding, fix, and roadmap item,
organized into an actual production plan. Written from all three seats established at the
start — Publisher, Investor, Solo Dev — because a production plan has to satisfy all three at
once or it isn't a real plan.

---

## 0. Executive Summary

**Ship the web-based Wallpaper product first.** It's the only one of three candidate products
that is verified, working code today. Treat the full C++ game as a separate, later, optional
bet — not a prerequisite.

**Three candidate products exist in this repo. Only one is real right now:**

| Candidate | Tech | Status |
|---|---|---|
| **Primordials — Living Wallpaper** | React/TS/Vite (`src/`) | ✅ **Verified working** — builds, boots, simulates, has a built Wallpaper Mode |
| Polygonal Primordials — full game | C++23 engine (`Engine/`, `Games/`) | ⬜ Unverified — never built in this session, 8-phase scope, huge |
| `PolygonalPrimordialsWallpaper.exe` | C++, referenced in `Tools/master_verify.py` | ⬜ **Doesn't appear to exist yet** — a build target the tooling checks for, with no corresponding source found |

**Recommendation, stated plainly:** the web Wallpaper product is 80% of the way to shippable
today. The native C++ wallpaper target is a *duplicate idea* that was never built. Don't build
it — it would be re-doing, in a much harder language/toolchain, something that already works
in the web codebase. Point all near-term production effort at the web product.

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
3. Wallpaper Engine's manifest/asset requirements (`project.json`, `preview.gif`/`preview.mp4`,
   size limits) — **still not yet researched**; this remains genuine next-step homework,
   distinct from `docs/WORKSHOP.md` in this repo, which documents Steam Workshop for the C++
   engine's theme files and does not apply here.

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
| Bolt.new AI-scaffold origin — unclear how much of the codebase/docs were AI-generated vs. hand-verified | Medium | Treat all unverified performance/scope claims (README's 1M-entity, deterministic-rollback claims) as marketing copy, not fact, until independently tested. Doesn't affect Product A, which has been directly verified this session. |
| Key-person risk — solo developer, no team redundancy | Standard for this project type | Not really mitigable at this stage; standard solo-dev risk, worth naming rather than ignoring in any future pitch |
| Placeholder/non-functional production tooling (`Tools/launch_checklist.py`'s hardcoded `True`) | Low direct risk, but erodes confidence in anything else in that pipeline | Don't rely on `Tools/` claims of "launch-ready" status without re-verifying each check individually — several appear to be scaffolding, not working checks |
| Dual-codebase, dual-pipeline confusion (§3) | Medium — wastes solo-dev time if not resolved | Resolved by this document: Product A is canonical, Product B/C are shelved |

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
- [ ] Wallpaper Engine manifest/preview assets prepared per their actual submission spec
      (genuine research task, not yet done)
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
