# Changelog

## [Phase E] — Machine Learning, Entity Behaviour, Environment & Controls

### Added

#### Machine Learning & AI (`Engine/AI/`, `Engine/Simulation/Systems/`)
- `NeuralNetwork.hpp`: Header-only multi-layer perceptron with Sigmoid/Tanh/ReLU activations, forward pass, and backpropagation
- `NeuralControlSystem`: `ISimulationSystem` driving entity actions via neural inference on `MindComponent` sensor data
- `QLearningSystem` + `QBrainComponent`: Tabular Q-learning with epsilon-greedy exploration, TD-update, bounds-safe Q-table access

#### Entity Behaviour (`Engine/ECS/`, `Engine/Simulation/Systems/`)
- `FlockComponent` (TypeId=16): Boids flock membership, perception radius, and per-component steering weights
- `GroupBehaviorSystem`: Craig Reynolds' Boids model — Separation, Alignment, Cohesion steering forces applied to `RigidBodyComponent`

#### Environment Simulation (`Engine/Simulation/Systems/`)
- `WeatherSystem`: 4-season cycle, dynamic weather transitions (Clear/Rain/Storm/Blizzard/Heatwave), temperature/humidity/wind model, `metabolic_drain_multiplier` and `resource_growth_multiplier` exported per-tick
- `BiomeSystem`: 5 spatial biomes (LushForest, AridDesert, Tundra, VolcanicCrater, DeepOcean) with position-queryable fertility, moisture, and terrain friction

#### Civilization & Buildings (`Engine/Simulation/Systems/`)
- `BuildingSystem` + `BuildingComponent`: Incremental construction progress, passive resource yields, `construct_building()` factory, `repair_building()` for structure health

#### Game Controls (`Engine/Simulation/`, `Engine/Rendering/`)
- `SimulationController`: Stopped/Running/Paused/StepSingleTick/FastForward state machine with 6 speed presets (0.25×–10×), synced to `SimulationClock` + `SimulationScheduler`
- `CameraController`: 2D viewport pan/zoom/entity-follow with `WorldToScreen`/`ScreenToWorld` coordinate projections
- `GameInputController`: `InputAction` mapper bridging user inputs to simulation and camera controls

#### Tests (`EngineTests/AI/`, `EngineTests/PhaseETests/`)
- `test_neural_network.cpp`, `test_qlearning.cpp`: ML system unit tests
- `test_group_behavior.cpp`: Boids steering correctness
- `test_weather.cpp`, `test_biome.cpp`: Environment system tests
- `test_building.cpp`: Construction and passive yield tests
- `test_simulation_controller.cpp`, `test_camera_controller.cpp`: Controls unit tests
- `test_end_to_end_user_scenarios.cpp`: 3 integrated user simulation scenarios

#### Documentation (`docs/`)
- `docs/03_Intelligence/29_Neural_ML_Systems.md`
- `docs/03_Intelligence/30_GroupBehavior_Boids.md`
- `docs/02_Simulation/21_WeatherSystem_Implementation.md`
- `docs/02_Simulation/22_BiomeSystem.md`
- `docs/05_Client/01_Simulation_Controls.md`

### Fixed
- `SpatialHashGrid::GetCellKey`: cast `x`,`y` to `u32` before shift to eliminate negative-coordinate sign-extension bug
- `QBrainComponent::get_q`/`set_q`: bounds-clamp both indices to Q-table dimensions
- `World::GetComponentStore<T>()`: switched from RTTI pointer keying to `u32 TypeId` via `m_StoresByTypeId` — eliminates cross-TU RTTI mismatches under static linkage
- `SentinelComponent`: explicit field initialization (`syncTimer=0.0f`, `syncInterval=0.5f`)

### Results
**183 test cases · 1,323 assertions · 0 failures**

---

## [Unreleased] — v2 cleanup

### Removed
- `LiveOps/` module (dashboard, anomaly, rollback, A/B testing)
- `Bots/Discord/` source
- Phase H (Live Operations) section from README — was describing deleted code
- `test_ecs.exe` and `build_fix.patch` from repo root (no provenance, binaries/patches don't belong in git)

### Changed
- README "Live Operations" section rewritten as "Planned (v2.x)" — the deleted code is now in the roadmap, not the present tense

## [1.0.0] - Phase G: Release
- Finalized Polygonal Primordials standalone game build structure
- Added Steamworks integration (Achievements, Workshop, Cloud Saves)
- Added telemetry and crash reporting
- Created release tools (Asset Cooker, Docs, Trailer, Localization)
- Built deployment pipeline via GitHub Actions
