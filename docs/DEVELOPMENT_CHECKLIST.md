# Primordials — Development Checklist

Actionable checklist, organized by the 6 phases from the unified roadmap. Each item references
the exact file(s) to touch and the test case(s) in `test-plan.md` that confirm it's done.
Check an item only when its linked test case(s) pass.

---

## Phase 0 — Close the Loop on Work Already Done

- [x] Visually confirm History panel shows the disabled-state message and disabled Save button
      with no `.env` present.
      **Files:** `src/components/HistoryPanel.tsx` (no code change expected, verification only)
      **Verifies:** TC-A4 ✅

---

## Phase 1 — Correctness Fixes (no design tradeoffs, any order, do first)

- [ ] Fix `ChemicalField.resize()` to copy forward existing `attractant`/`repellent` data
      instead of reallocating empty arrays.
      **Files:** `src/sim/chemicalField.ts`
      **Verifies:** TC-D2

- [x] Fix Wallpaper Mode settings-clobbering bug — replaced the passive `useEffect` with
      explicit `handleThemeChange`/`handlePacingChange` handlers; settings now only change on
      actual user selection, never as a side effect of toggling `wallpaperMode`.
      **Files:** `src/App.tsx`
      **Verifies:** TC-F2 ✅

- [ ] Wire brain outputs `[3]` and `[4]` into aggression/social behavior (attack commitment,
      colony-joining intent).
      **Files:** `src/sim/simulation.ts` (consumption), no change to `brain.ts` needed
      **Verifies:** TC-C1

---

## Phase 2 — Foundational (unblocks the most future work)

- [ ] Add a spatial index (grid hash, cell size ≈ largest relevant interaction radius) for
      organism-proximity queries; refactor altruism, competition, and hunting-target loops in
      `updateOrganism`/`processSocialBehavior` to use it instead of full O(n²) scans.
      **Files:** new `src/sim/spatialGrid.ts` (or similar), `src/sim/simulation.ts`
      **Verifies:** TC-E1

- [ ] Fix brain tier-crossing weight loss — pad brains to a fixed max hidden size (8) and mask
      unused units per intelligence tier, so `mutateBrainForGenome`/`crossoverBrainForGenome`
      never need to discard evolved weights on a size mismatch.
      **Files:** `src/sim/brain.ts`, `src/sim/genetics.ts`
      **Verifies:** TC-C2

- [x] Build the Ecosystem Mood aggregation layer — compute scene-level scalars (population
      trend, dominant species hue, recent leap/speciation events, disease-outbreak state) from
      `sim.stats` on a periodic cadence, and feed them into `drawAmbientParticles` and the biome
      background gradient.
      **Files:** `src/sim/simulation.ts` (aggregation), `src/sim/renderer.ts` (consumption)
      **Verifies:** TC-G1 ✅, TC-G2 ✅

---

## Phase 3 — Entity Legibility

- [ ] Hook `hadEvolutionLeap()` to a renderer effect — brief glow pulse or particle burst on the
      organism at the tick a leap is detected, fading afterward.
      **Files:** `src/sim/simulation.ts` (detect + flag), `src/sim/renderer.ts` (render)
      **Verifies:** TC-B1

- [ ] Add a categorical visual distinction for `diet` (outline style or glyph), separate from
      the continuous hue/size/sides channels.
      **Files:** `src/sim/renderer.ts`
      **Verifies:** TC-B2

- [ ] Wire `socialRank` into at least one real behavior (e.g. alphas immune to same-colony
      competition tax, or preferential altruism targeting).
      **Files:** `src/sim/simulation.ts` (`processSocialBehavior`)
      **Verifies:** TC-E2

- [ ] Decouple minimal flee reflex from the `intel >= 0.2` brain-existence gate — give
      brainless organisms a crude "move away from nearest large aggressive organism" instinct.
      **Files:** `src/sim/simulation.ts` (`updateOrganism` threat detection block)
      **Verifies:** TC-E3

- [ ] Add an `intelligence`-proportional term to `applyEnergyCost`, scaled by
      `hiddenForIntel(intelligence)`.
      **Files:** `src/sim/simulation.ts`
      **Verifies:** TC-C3

---

## Phase 4 — Environment Depth

- [ ] Remove directional bias in `expandWorld()` — alternate or randomize which
      quadrant/direction each expansion grows into, and/or randomize new-biome placement across
      the full map rather than only the newest 40%.
      **Files:** `src/sim/simulation.ts` (`expandWorld`)
      **Verifies:** TC-D1

- [ ] Make `CinematicCamera`'s idle/auto zoom a function of current `worldWidth`/`worldHeight`
      instead of a fixed constant.
      **Files:** `src/sim/cinematicCamera.ts`
      **Verifies:** TC-D3

- [ ] Add falloff-based biome edge blending — interpolate `energyDrain`/`speedMod`/visual hue
      across a boundary zone instead of snapping at `radius`.
      **Files:** `src/sim/simulation.ts` (`biomeAt` and stat application), `src/sim/renderer.ts`
      (visual blending)
      **Verifies:** TC-D4

- [ ] (Stretch) Add a day/night or seasonal light cycle animating the existing per-biome
      `light` scalar over time, consumed by `photosynthesis`.
      **Files:** `src/sim/simulation.ts`, `src/sim/renderer.ts`

---

## Phase 5 — Bigger Bets

- [x] Tie hue mutation/inheritance to `geneticDistance()` so visually similar color correlates
      with genetic closeness, not pure random drift.
      **Files:** `src/sim/genetics.ts` (`mutateGenome` — restructured, hue computed last from
      a distance-scaled mutation amount)
      **Verifies:** TC-B3 ✅ (empirically verified via two standalone statistical tests against
      the real production functions, both directions: single-mutation and multi-generation)

- [ ] Give the brain real strategic influence — expand inputs/outputs to cover prey-selection
      scoring and social genes (`altruism`, `competitiveness`, `dominance`, `cooperation`,
      `socialGene`), so evolved weights can affect *who*/*whether* decisions, not just movement.
      **Files:** `src/sim/brain.ts` (`N_INPUTS`/`N_OUTPUTS`), `src/sim/genetics.ts`,
      `src/sim/simulation.ts` (`collectBrainInputs`, hunting/social loops)
      **Note:** changes brain shape — sequence after Phase 2's tier-crossing fix is validated,
      since this touches the same weight-compatibility system.

- [ ] Build the entity→environment feedback loop — colony/population activity locally alters
      biome stats over time (overgrazing lowers local `foodRate`, colony density raises local
      `energyDrain`, etc.), instead of biomes only ever affecting entities.
      **Files:** `src/sim/simulation.ts`, `src/sim/types.ts` (`Biome` may need mutable
      per-instance state distinct from `BIOME_INFO` template values)

---

## Phase 6 — Polish

- [x] Add Wallpaper Mode click-feedback — lightweight ambient-styled popover on organism click
      (species, generation, 1-2 standout traits), matching the `AmbientHUD`/`WallpaperDock`
      glass aesthetic. Alternative: disable click entirely if the popover is out of scope.
      **Files:** `src/App.tsx`, `src/components/WallpaperInfoPopover.tsx`
      **Verifies:** TC-F3 ✅

- [ ] Surface a read-only summary in Wallpaper Mode of which sim-only settings are currently
      active (knowledge nodes, structures, disease events, etc.).
      **Files:** `src/components/WallpaperDock.tsx` or `AmbientHUD.tsx`
      **Verifies:** TC-F1

---

## Cross-Cutting Reminders (apply throughout, not phase-specific)

- [ ] Run `npm run typecheck` and `npm run build` after every change in this checklist —
      both must stay clean (per TC-A2/TC-A3) before moving to the next item.
- [ ] Any new/promoted genome trait gets an assigned visual channel before it ships (per the
      entity-design doc's channel-allocation table) — don't add invisible traits.
- [ ] Any new togglable feature extends the existing `SimSettings` type and
      `onToggleSetting(key, value)` pattern — no parallel settings systems.
- [ ] Any new Wallpaper Mode UI element matches the existing glassmorphic tokens
      (`bg-neutral-950/70`, `backdrop-blur-xl`, `ring-1 ring-white/10`) and the idle-fade
      behavior already used by `AmbientHUD`/`WallpaperDock`.
