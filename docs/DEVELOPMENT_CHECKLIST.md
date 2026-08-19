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

- [x] Fix `ChemicalField.resize()` to copy forward existing `attractant`/`repellent` data
      instead of reallocating empty arrays.
      **Files:** `src/sim/chemicalField.ts`
      **Verifies:** TC-D2 ✅ (empirically verified via scratch/test_tcd2.ts with 0% data loss)

- [x] Fix Wallpaper Mode settings-clobbering bug — replaced the passive `useEffect` with
      explicit `handleThemeChange`/`handlePacingChange` handlers; settings now only change on
      actual user selection, never as a side effect of toggling `wallpaperMode`.
      **Files:** `src/App.tsx`
      **Verifies:** TC-F2 ✅

- [x] Wire brain outputs `[3]` and `[4]` into aggression/social behavior (attack commitment,
      colony-joining intent).
      **Files:** `src/sim/simulation.ts` (threat/flee block, altruism block in
      `processSocialBehavior`)
      **Verifies:** TC-C1 ✅ (empirically verified via controlled deterministic-brain test
      against the real `Simulation` class — see test-plan.md for exact numbers)

---

## Phase 2 — Foundational (unblocks the most future work)

- [x] Add a spatial index (`SpatialHashGrid` in `spatialHash.ts`) for organism-proximity queries;
      refactor altruism, competition, hunting, mating, and disease loops in
      `updateOrganism`/`processSocialBehavior` to use it instead of full O(n²) scans.
      **Files:** `src/sim/spatialHash.ts`, `src/sim/simulation.ts`
      **Verifies:** TC-E1 ✅ (empirically verified sub-quadratic scaling: 5.14x tick time ratio
      from 209 to 984 population)

- [x] Fix brain tier-crossing weight loss — pad brains to a fixed max hidden size (MAX_HIDDEN = 8) and mask
      unused units per intelligence tier (`activeHidden`), so `mutateBrainForGenome`/`crossoverBrainForGenome`
      never need to discard evolved weights on a size mismatch.
      **Files:** `src/sim/brain.ts`, `src/sim/genetics.ts`, `src/sim/simulation.ts`, `src/components/InspectorPanel.tsx`
      **Verifies:** TC-C2 ✅

- [x] Build the Ecosystem Mood aggregation layer — compute scene-level scalars (population
      health, dominant species hue, recent leap-event intensity) and feed them into
      `drawAmbientParticles`.
      **Files:** `src/sim/renderer.ts` (both aggregation and consumption live in `render()`,
      no `simulation.ts` changes needed — all data already existed on `sim`)
      **Verifies:** TC-G1 ✅, TC-G2 ✅ (found already implemented this pass; corrected stale
      docs and re-verified TC-G1 live via a forced population-crash screenshot comparison)

---

## Phase 3 — Entity Legibility

- [x] Hook `hadEvolutionLeap()` to a renderer effect — brief glow pulse or particle burst on the
      organism at the tick a leap is detected, fading afterward.
      **Files:** `src/sim/simulation.ts` (detect + flag), `src/sim/renderer.ts` (render `drawStar` sparkle particle)
      **Verifies:** TC-B1 ✅

- [x] Add a categorical visual distinction for `diet` (outline style or glyph), separate from
      the continuous hue/size/sides channels.
      **Files:** `src/sim/renderer.ts`
      **Verifies:** TC-B2 ✅ (carnivore double outer stroke, corner tooth spikes, and inner polygon vs. herbivore smooth membrane)

- [x] Wire `socialRank` into at least one real behavior (alphas immune to same-colony
      competition tax and preferential 1.4x altruism targeting).
      **Files:** `src/sim/simulation.ts` (`processSocialBehavior`)
      **Verifies:** TC-E2 ✅ (empirically verified via scratch/test_tce2.ts)

- [x] Decouple minimal flee reflex from the `intel >= 0.2` brain-existence gate — give
      brainless organisms a crude "move away from nearest large aggressive organism" instinct.
      **Files:** `src/sim/simulation.ts` (`updateOrganism` threat detection block)
      **Verifies:** TC-E3 ✅ (empirically verified via scratch/test_tce3.ts)

- [x] Add an `intelligence`-proportional term to `applyEnergyCost`, scaled by
      `hiddenForIntel(intelligence)`.
      **Files:** `src/sim/simulation.ts`
      **Verifies:** TC-C3 ✅ (empirically verified via scratch/test_tcc3.ts: 1.16x higher drain)

---

## Phase 4 — Environment Depth

- [x] Remove directional bias in `expandWorld()` — alternate or randomize which
      quadrant/direction each expansion grows into, and/or randomize new-biome placement across
      the full map rather than only the newest 40%.
      **Files:** `src/sim/simulation.ts` (`expandWorld`)
      **Verifies:** TC-D1 ✅ (biome placement uniform across full world)

- [x] Make `CinematicCamera`'s idle/auto zoom a function of current `worldWidth`/`worldHeight`
      instead of a fixed constant.
      **Files:** `src/sim/cinematicCamera.ts`
      **Verifies:** TC-D3 ✅ (monotonic decrease verified across 10 expansions with floor-clamping)

- [x] Add falloff-based biome edge blending — interpolate `energyDrain`/`speedMod`/visual hue
      across a boundary zone instead of snapping at `radius`.
      **Files:** `src/sim/simulation.ts` (`biomeBlendAt` and stat application), `src/sim/renderer.ts`
      (`drawBiomeBoundaryBlend` visual blending)
      **Verifies:** TC-D4 ✅ (smooth quadratic falloff for energy/speed + soft hue-blend radial gradient overlay)

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

- [x] Give the brain real strategic influence — expand inputs/outputs to cover prey-selection
      scoring and social genes (`altruism`, `competitiveness`, `dominance`, `cooperation`,
      `socialGene`), so evolved weights can affect *who*/*whether* decisions, not just movement.
      **Files:** `src/sim/brain.ts` (`N_INPUTS`/`N_OUTPUTS`), `src/sim/genetics.ts`,
      `src/sim/simulation.ts` (`collectBrainInputs`, hunting/social loops), `src/components/InspectorPanel.tsx`
      **Verifies:** Phase 5a ✅ (N_INPUTS: 22, N_OUTPUTS: 7; output[5] biases prey selection strategy, output[6] controls colony formation and joining willingness)

- [x] Build the entity→environment feedback loop — colony/population activity locally alters
      biome stats over time (overgrazing lowers local `foodRate`, colony density raises local
      `energyDrain`, etc.), instead of biomes only ever affecting entities.
      **Files:** `src/sim/simulation.ts` (`updateBiomeFeedback`)
      **Verifies:** Phase 5b ✅ (herbivore grazing reduces foodRate down to 0.3x, density raises energyDrain up to 2.0x, photosynthesis boosts foodRate up to 1.8x, with 3x asymmetric recovery lag)

---

## Phase 6 — Polish

- [x] Add Wallpaper Mode click-feedback — lightweight ambient-styled popover on organism click
      (species, generation, 1-2 standout traits), matching the `AmbientHUD`/`WallpaperDock`
      glass aesthetic. Alternative: disable click entirely if the popover is out of scope.
      **Files:** `src/App.tsx`, `src/components/WallpaperInfoPopover.tsx`
      **Verifies:** TC-F3 ✅

- [x] Surface a read-only summary in Wallpaper Mode of which sim-only settings are currently
      active (knowledge nodes, structures, disease events, etc.).
      **Files:** `src/components/WallpaperDock.tsx` or `AmbientHUD.tsx`
      **Verifies:** TC-F1 ✅

---

## Cross-Cutting Reminders (apply throughout, not phase-specific)

- [x] Run `npm run typecheck` and `npm run build` after every change in this checklist —
      both must stay clean (per TC-A2/TC-A3) before moving to the next item. (Verified clean: 0 TS errors, clean production bundle)
- [x] Any new/promoted genome trait gets an assigned visual channel before it ships (per the
      entity-design doc's channel-allocation table) — don't add invisible traits.
- [x] Any new togglable feature extends the existing `SimSettings` type and
      `onToggleSetting(key, value)` pattern — no parallel settings systems.
- [x] Any new Wallpaper Mode UI element matches the existing glassmorphic tokens
      (`bg-neutral-950/70`, `backdrop-blur-xl`, `ring-1 ring-white/10`) and the idle-fade
      behavior already used by `AmbientHUD`/`WallpaperDock`.
