# Primordials — Final Test Plan

Consolidates every fix and finding surfaced across this session (Supabase fix, entity design,
ML/neural, open world, neural-behavioral, control modes, Ecosystem Mood) into concrete,
checkable test cases. Status reflects what was actually verified in this session, not assumed —
where something was implemented but not re-confirmed visually, that's called out explicitly
rather than marked as a pass.

**Status key:** ✅ Verified this session · 🔶 Implemented, not visually re-verified · ⬜ Not yet implemented

---

## A. Boot & Configuration

### TC-A1 — Fresh clone boots with no `.env` file
- **Preconditions:** Repo cloned, `npm install` run, no `.env` present.
- **Steps:** `npm run build` → `npm run preview` → load root URL in a browser.
- **Expected:** App renders the running simulation (Species panel, live population count).
  No blank white screen, no unhandled `pageerror` in console.
- **Status:** ✅ Verified — confirmed via headless Chromium, zero `.env`, screenshot showed live
  sim UI, no `supabaseUrl is required` error in console.

### TC-A2 — `npm run typecheck` passes with zero errors
- **Steps:** `npm run typecheck` from repo root.
- **Expected:** Exits clean, no TS errors.
- **Status:** ✅ Verified — re-run after all `supabase.ts`/`App.tsx`/`HistoryPanel.tsx` edits.

### TC-A3 — `npm run build` succeeds with and without `.env`
- **Steps:** Run build once with `.env` present (dummy values), once with `.env` absent.
- **Expected:** Both produce a clean `dist/` bundle, no build errors.
- **Status:** ✅ Verified — both cases built cleanly (1,563 modules transformed each time).

### TC-A4 — History panel communicates disabled state when Supabase isn't configured
- **Preconditions:** No `.env` present.
- **Steps:** Open the app → navigate to the History tab/panel.
- **Expected:** "Cloud history is disabled — this build isn't connected to a Supabase project."
  message shown; Save button rendered disabled (not just silently failing on click).
- **Status:** ✅ Verified — Experiments tab (the actual UI label for history) screenshotted with
  no `.env` present. Shows "Cloud history is disabled..." message, Save button visibly disabled,
  rest of the app (sim, all toggles) fully unaffected.

### TC-A5 — `.env.example` documents optional Supabase config
- **Steps:** Inspect `.env.example` at repo root.
- **Expected:** File exists, explains Supabase is optional and only affects cloud history.
- **Status:** ✅ Verified — file created and content confirmed.

---

## B. Entity Visual Design

### TC-B1 — Evolution leap produces a visible effect
- **Preconditions:** `hadEvolutionLeap()` wired to a renderer effect (not yet implemented).
- **Steps:** Run sim at high speed until a leap occurs (watch `stats` for a leap event) →
  observe the leaping organism at the moment of mutation.
- **Expected:** A distinct, temporary visual (glow pulse or particle burst) plays on the
  organism at the tick the leap is detected, then fades — not a permanent marker.
- **Status:** ⬜ Not yet implemented.

### TC-B2 — Diet is visually distinguishable without opening the Inspector
- **Steps:** Observe a mixed population of herbivores and carnivores on canvas.
- **Expected:** Carnivores are visually distinct (outline style, glyph, or similar categorical
  marker) from herbivores at a glance.
- **Status:** ⬜ Not yet implemented — currently only `sides`/`size`/`hue` are visually mapped;
  `diet` has no dedicated visual channel.

### TC-B3 — Hue correlates with genetic distance between related species
- **Steps:** Let two species diverge from a common ancestor over many generations → compare
  their hues to two unrelated, distantly-related species.
- **Expected:** Recently-diverged species are visually closer in hue than distantly-related
  ones; hue drift is not purely random relative to `geneticDistance()`.
- **Status:** ⬜ Not yet implemented — hue mutation is currently independent of
  `geneticDistance()`.

### TC-B4 — Wallpaper Mode enforces a per-theme population ceiling independent of sim view
- **Steps:** Set a high `maxPopulation` in Simulation View → switch to Wallpaper Mode with a
  theme that should cap population lower.
- **Expected:** Wallpaper Mode population stays within the active theme's intended ceiling,
  regardless of the Simulation View value that was set beforehand.
- **Status:** ⬜ Not yet implemented as an explicit guarantee — currently entangled with the
  settings-clobbering bug in TC-F2 below; fixing F2 should be validated against this case too.

---

## C. Neural Brain (ML) System

### TC-C1 — Brain outputs `[3]` and `[4]` have an observable effect
- **Preconditions:** Outputs wired to aggression/social behavior per the ML-parameters doc.
- **Steps:** Compare two organisms with identical genomes except divergent evolved weights on
  outputs 3/4 → observe behavior difference (e.g. attack commitment, colony-joining tendency).
- **Expected:** Measurable behavioral difference attributable to those two outputs.
- **Status:** ⬜ Not yet implemented — currently outputs 3/4 are computed every tick and never
  read anywhere in `simulation.ts`.

### TC-C2 — Brain weights survive an intelligence tier crossing
- **Preconditions:** Padding or weight-preserving resize implemented per §2 of the ML doc.
- **Steps:** Track a single lineage's brain weights across a mutation that crosses a
  `hiddenForIntel` tier boundary (e.g. intelligence 0.39 → 0.41).
- **Expected:** Pre-crossing evolved weights are preserved (or meaningfully carried forward),
  not replaced with a fresh random brain.
- **Status:** ⬜ Not yet implemented — current `mutateBrainForGenome`/`crossoverBrainForGenome`
  discard weights entirely on any hidden-layer size mismatch.

### TC-C3 — Intelligence has a measurable metabolic cost
- **Preconditions:** Cost term added to `applyEnergyCost` per §3 of the ML doc.
- **Steps:** Compare energy drain per tick between two organisms identical except for
  `intelligence` (and resulting brain size).
- **Expected:** Higher-intelligence organism drains energy faster, proportional to
  `hiddenForIntel(intelligence)`.
- **Status:** ⬜ Not yet implemented — `applyEnergyCost` currently has no `intelligence` term.

### TC-C4 — Regression: brain input normalization stays within expected ranges
- **Steps:** Instrument `collectBrainInputs` (or inspect via `org.lastInputs` in the Inspector)
  across a range of organism states, including edge cases (zero energy, no food/threat in
  range).
- **Expected:** All 14 inputs remain within their documented ranges (0–1 scalars, unit-vector
  direction pairs; zeroed when no food/threat found) — no `NaN`/`Infinity` leaking into
  `evalBrain`.
- **Status:** ⬜ Not run this session — worth adding as an automated regression check given how
  central this function is, especially once C1–C3 start modifying the surrounding code.

---

## D. Open World & Environment

### TC-D1 — World expansion is not directionally biased
- **Preconditions:** Fix from §1 of the open-world doc applied (alternating/randomized growth
  direction, or full coordinate-offset rework).
- **Steps:** Let the world expand many times (Endless Generation, long run) → plot biome
  centers across all expansions.
- **Expected:** New biomes distributed roughly evenly around the existing world, not clustered
  in one quadrant.
- **Status:** ⬜ Not yet implemented — current `expandWorld()` always grows worldWidth/Height
  upward and places new biomes in the `[0.6×, 1.0×]` range of the new dimensions.

### TC-D2 — Chemical field data survives a world expansion
- **Preconditions:** `ChemicalField.resize()` updated to copy forward existing cell data.
- **Steps:** Build up a pheromone gradient (let chemotaxis-driven organisms deposit
  attractant/repellent) → trigger a world expansion → inspect `attractant`/`repellent` arrays
  immediately after.
- **Expected:** Previously-deposited gradient values at unchanged coordinates are preserved,
  not zeroed.
- **Status:** ⬜ Not yet implemented — current `resize()` reallocates fresh empty
  `Float32Array`s unconditionally.

### TC-D3 — Camera "whole world" framing adapts as the world grows
- **Preconditions:** Zoom-scaling fix from §3 of the open-world doc applied.
- **Steps:** Let the world expand several times → observe idle/auto camera zoom level before
  and after.
- **Expected:** Zoom level adjusts so a meaningfully larger fraction of the world remains
  visible as world size grows, rather than staying fixed at the original `0.85`.
- **Status:** ⬜ Not yet implemented.

### TC-D4 — Biome transitions are not hard stat snaps
- **Preconditions:** Edge-blending implemented per §4.1 of the open-world doc.
- **Steps:** Move an organism slowly across a biome boundary → sample its effective
  `energyDrain`/`speedMod` at several points near the edge.
- **Expected:** Stats interpolate smoothly across a falloff zone rather than flipping instantly
  at the boundary radius.
- **Status:** ⬜ Not yet implemented.

---

## E. Behavioral / Social Systems

### TC-E1 — Instinct proximity loops scale sub-quadratically
- **Preconditions:** Spatial index implemented per §2 of the neural-behavioral doc.
- **Steps:** Profile tick duration for the altruism/competition/hunting loops at population =
  200, 600, 1200.
- **Expected:** Per-tick cost grows roughly linearly with population, not quadratically —
  concretely, tick time at 1200 population should not be ~36× the tick time at 200 (which is
  what O(n²) would predict; near-linear should look closer to ~6×).
- **Status:** ⬜ Not yet implemented / not yet profiled.

### TC-E2 — Social rank has a behavioral effect
- **Preconditions:** At least one hook from §3 of the neural-behavioral doc implemented (e.g.
  alphas immune to same-colony competition tax).
- **Steps:** Compare outcomes for an alpha vs. an omega in the same colony under identical
  external conditions.
- **Expected:** Measurable behavioral or outcome difference tied to `socialRank`, beyond what's
  shown in the Inspector.
- **Status:** ⬜ Not yet implemented — `socialRank` is currently computed and displayed only.

### TC-E3 — Low-intelligence organisms have some baseline flee behavior
- **Preconditions:** Minimal flee reflex decoupled from the `intel >= 0.2` gate per §4 of the
  neural-behavioral doc.
- **Steps:** Spawn a fresh random-genome organism with `intelligence < 0.2` near a large
  aggressive predator.
- **Expected:** Organism exhibits at least crude avoidance (movement away from the threat),
  rather than zero flee behavior.
- **Status:** ⬜ Not yet implemented — currently threat detection is fully gated at `intel >=
  0.2`, identical to the brain-existence threshold.

---

## F. Control Modes & Wallpaper Mode

### TC-F1 — Sim-only settings are visible (even if read-only) in Wallpaper Mode
- **Preconditions:** Summary UI from §2 of the control-modes doc added.
- **Steps:** Enable a sim-only feature (e.g. Disease Events) in Simulation View → switch to
  Wallpaper Mode.
- **Expected:** Wallpaper Mode UI indicates Disease Events is active, even though there's no
  toggle for it there.
- **Status:** ⬜ Not yet implemented.

### TC-F2 — Entering Wallpaper Mode does not silently overwrite tuned sim settings
- **Preconditions:** Fix from §3 of the control-modes doc applied.
- **Steps:** In Simulation View, set `maxPopulation` and `mutationRate` to custom non-default
  values → switch to Wallpaper Mode → inspect `sim.settings`.
- **Expected:** Custom values are preserved, or the user is clearly informed/prompted that
  switching modes will apply theme/pacing defaults — not a silent overwrite.
- **Status:** ✅ **Fixed and verified.** Replaced the passive `useEffect` (keyed on
  `wallpaperMode`) with explicit `handleThemeChange`/`handlePacingChange` handlers — settings
  now only change when the user actually picks a theme or pacing preset. Verified live:
  `maxPopulation=750` set in Simulation View survived entering Wallpaper Mode unchanged;
  separately, forcing an arbitrary value then explicitly picking "Crystal Cave" correctly
  applied that theme's real default (200) — confirming explicit selection still works.

### TC-F3 — Clicking an organism in Wallpaper Mode produces visible feedback
- **Preconditions:** Fix from §4 of the control-modes doc applied (popover or click disabled).
- **Steps:** In Wallpaper Mode, click a visible organism.
- **Expected:** Either a lightweight info popover appears (preferred), or the canvas does not
  invite a click at all (cursor/affordance signals non-interactivity) — not the current silent
  no-op.
- **Status:** ✅ **Fixed and verified.** Built `WallpaperInfoPopover.tsx` — a lightweight,
  glassmorphic info card (species, shape, diet, generation, mind tier, species population)
  matching the existing `AmbientHUD`/`WallpaperDock` visual language. Wired to the same
  `onSelect` the canvas already fired. Verified live via a coordinate-grid click sweep: (1)
  clicking an organism shows the popover with correct real data, (2) the X button closes it,
  (3) clicking empty space auto-dismisses it (no extra code needed — `SimCanvas` already
  returns `null` when no organism is under the click).

---

## G. Ecosystem Mood (Atmosphere Reactivity)

### TC-G1 — Ambient particle layer responds to population health
- **Preconditions:** Ecosystem Mood aggregation layer implemented per the unified roadmap.
- **Steps:** Compare ambient particle density/behavior during a population boom vs. a
  population collapse (e.g. after a disease outbreak).
- **Expected:** Observable difference in the atmosphere layer correlated with population
  trend — not currently the case.
- **Status:** ⬜ Not yet implemented — confirmed `drawAmbientParticles` currently takes no
  simulation-state input, only `phase` (time) and static theme config.

### TC-G2 — Speciation/leap events produce an atmosphere-level moment
- **Preconditions:** Ecosystem Mood wired to leap/speciation events.
- **Steps:** Trigger (or wait for) a new species formation or evolution leap → observe the
  wider scene, not just the individual organism.
- **Expected:** A brief, scene-level acknowledgment (subtle pulse, particle response) distinct
  from steady-state atmosphere.
- **Status:** ⬜ Not yet implemented.

---

## Summary

| Area | ✅ Verified | 🔶 Implemented, unverified | ⬜ Not implemented |
|---|---|---|---|
| A — Boot & Config | 5 | 0 | 0 |
| B — Entity Visuals | 0 | 0 | 4 |
| C — Neural/ML | 0 | 0 | 4 |
| D — Open World | 0 | 0 | 4 |
| E — Behavioral | 0 | 0 | 3 |
| F — Control Modes | 2 | 0 | 1 |
| G — Ecosystem Mood | 0 | 0 | 2 |
| **Total** | **7** | **0** | **18** |

**All confirmed live bugs from this session are now fixed.** Remaining items (18) are the
deeper Phase 2–6 roadmap features from `unified-visual-environment-entity-roadmap.md` —
none are launch-blocking per `production-development-plan.md` §8.
