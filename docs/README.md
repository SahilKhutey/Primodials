# Shape Engine — Simulation Bible Index

Welcome to the **Shape Engine Simulation Bible**. This library is the single source of truth for the entire engine design, physics layers, biological pipelines, cognitive pipelines, social networks, and data-oriented architectural patterns.

---

## Systems Engineering Blueprints (M0 Deliverables) ✅
*This section contains the 21 system engineering design artifacts mapping all engine modules, mathematical models, memory layouts, and implementation roadmaps.*

* [Architecture](file:///C:/Users/ASUS/Documents/Primodilas/docs/06_Design/Architecture.md) — Overall engine layout and layering.
* [Systems](file:///C:/Users/ASUS/Documents/Primodilas/docs/06_Design/Systems.md) — The 16 core systems specification sheets.
* [Modules](file:///C:/Users/ASUS/Documents/Primodilas/docs/06_Design/Modules.md) — Module interface dependency logs (no cycles).
* [Framework](file:///C:/Users/ASUS/Documents/Primodilas/docs/06_Design/Framework.md) — Core layered execution frameworks.
* [Simulation](file:///C:/Users/ASUS/Documents/Primodilas/docs/06_Design/Simulation.md) — Heartbeat clock and ticks order schedulers.
* [Mathematics](file:///C:/Users/ASUS/Documents/Primodilas/docs/06_Design/Mathematics.md) — Vector math, Gauss area formulas, and optimization models.
* [Physics](file:///C:/Users/ASUS/Documents/Primodilas/docs/06_Design/Physics.md) — Kinematics and SAT impulses resolution physics.
* [Biology](file:///C:/Users/ASUS/Documents/Primodilas/docs/06_Design/Biology.md) — Genetics, metabolic lifecycles, and Lotka-Volterra dynamics.
* [AI](file:///C:/Users/ASUS/Documents/Primodilas/docs/06_Design/AI.md) — Cognition, attention filters, and Utility AI motivations.
* [Civilization](file:///C:/Users/ASUS/Documents/Primodilas/docs/06_Design/Civilization.md) — Economics supply curves, research, and diplomacy.
* [Optimization](file:///C:/Users/ASUS/Documents/Primodilas/docs/06_Design/Optimization.md) — Cache guidelines, thread safety, and LOD tick scaling.
* [DataStructures](file:///C:/Users/ASUS/Documents/Primodilas/docs/06_Design/DataStructures.md) — Contiguous memory alignments, layouts, and serializations.
* [Algorithms](file:///C:/Users/ASUS/Documents/Primodilas/docs/06_Design/Algorithms.md) — Flocking, pathfindings, and spatial grid mappings.
* [Memory](file:///C:/Users/ASUS/Documents/Primodilas/docs/06_Design/Memory.md) — custom Arenas and Pools, zero runtime heap contracts.
* [Threading](file:///C:/Users/ASUS/Documents/Primodilas/docs/06_Design/Threading.md) — Parallel job systems, spatial workers, task graphs.
* [Rendering](file:///C:/Users/ASUS/Documents/Primodilas/docs/06_Design/Rendering.md) — Decoupled render graph, extraction, and graphics wrappers.
* [Networking](file:///C:/Users/ASUS/Documents/Primodilas/docs/06_Design/Networking.md) — Lockstep packets syncs, CRC32 hashes, and rollback replays.
* [Serialization](file:///C:/Users/ASUS/Documents/Primodilas/docs/06_Design/Serialization.md) — Bounded binary snapshot files format layouts.
* [Testing](file:///C:/Users/ASUS/Documents/Primodilas/docs/06_Design/Testing.md) — Catch2 unit, integration, and fuzz verification suites.
* [Benchmarks](file:///C:/Users/ASUS/Documents/Primodilas/docs/06_Design/Benchmarks.md) — Performance metric profiles and cache profiling.
* [Roadmap](file:///C:/Users/ASUS/Documents/Primodilas/docs/06_Design/Roadmap.md) — Epics logs (EPIC-001 to EPIC-015) and 15-stage pipelines.

---

## Subsystem Specifications (SRS M1 Contracts) ✅
*This section contains the 16 Software Requirements Specifications (SRS) blueprints for every engine system, including verification and validation (V&V) procedures.*

* [01_Core_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/08_Specifications/01_Core_System.md) — Compiler abstractions, basic types, static assertions.
* [02_Platform_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/08_Specifications/02_Platform_System.md) — SDL3 wrappers for inputs unprojection, clock services.
* [03_Memory_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/08_Specifications/03_Memory_System.md) — custom Arenas and Pools, zero allocation contracts.
* [04_Math_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/08_Specifications/04_Math_System.md) — Vector2, Shoelace area integration, LCG RNG states.
* [05_Container_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/08_Specifications/05_Container_System.md) — Contiguous dense-sparse sets registries, command queue.
* [06_Utility_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/08_Specifications/06_Utility_System.md) — Event queues, config properties, hashing.
* [07_Reflection_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/08_Specifications/07_Reflection_System.md) — Typenames mapping and metadata.
* [08_Logging_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/08_Specifications/08_Logging_System.md) — Async logger buffers, locking exclusion.
* [09_Job_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/08_Specifications/09_Job_System.md) — Parallel spatial worker pools, work stealing.
* [10_Simulation_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/08_Specifications/10_Simulation_System.md) — Heartbeat clock schedulers, tick sequential loops.
* [11_Physics_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/08_Specifications/11_Physics_System.md) — Euler-Cromer kinematics, SAT collisions, Baumgarte.
* [12_AI_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/08_Specifications/12_AI_System.md) — Perception FOVs, memory log decay, Utility AI decisions.
* [13_Civilization_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/08_Specifications/13_Civilization_System.md) — Inventories logistics, research technology trees, pricing.
* [14_Rendering_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/08_Specifications/14_Rendering_System.md) — Decoupled render graph pipelines, frame interpolation.
* [15_Networking_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/08_Specifications/15_Networking_System.md) — Lockstep packets synchronization, rollback re-simulation.
* [16_Editor_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/08_Specifications/16_Editor_System.md) — ImGui inspector panels, dev overlays, diagnostics timelines.

---

## Volume I — Foundations (Immutable Universe Laws) ✅
This volume defines the mathematics, chemistry, physics, and thermodynamic principles governing every simulation tick.

* [01_Project_Vision](file:///C:/Users/ASUS/Documents/Primodilas/docs/01_Foundations/01_Project_Vision.md) — Long-term sandbox objectives.
* [02_Design_Philosophy](file:///C:/Users/ASUS/Documents/Primodilas/docs/01_Foundations/02_Design_Philosophy.md) — Emergence over scripting guidelines.
* [03_Universe_Principles](file:///C:/Users/ASUS/Documents/Primodilas/docs/01_Foundations/03_Universe_Principles.md) — Conservation of mass, energy, and information.
* [04_Universal_Axioms](file:///C:/Users/ASUS/Documents/Primodilas/docs/01_Foundations/04_Universal_Axioms.md) — Immutable rules of entity states.
* [05_Mathematical_Framework](file:///C:/Users/ASUS/Documents/Primodilas/docs/01_Foundations/05_Mathematical_Framework.md) — Vectors, areas, centroids, and moments of inertia.
* [06_Physics_Framework](file:///C:/Users/ASUS/Documents/Primodilas/docs/01_Foundations/06_Physics_Framework.md) — Euler-Cromer integration and SAT collisions.
* [07_Chemistry_Framework](file:///C:/Users/ASUS/Documents/Primodilas/docs/01_Foundations/07_Chemistry_Framework.md) — Molecular states, Arrhenius kinetics, and fluid thermodynamics.
* [08_Biology_Framework](file:///C:/Users/ASUS/Documents/Primodilas/docs/01_Foundations/08_Biology_Framework.md) — Genetics vectors, crossovers, and reproduction lifecycles.
* [09_Geometry_Framework](file:///C:/Users/ASUS/Documents/Primodilas/docs/01_Foundations/09_Geometry_Framework.md) — Convex hulls, Voronoi partitioning, and mesh caches.
* [10_Time_Framework](file:///C:/Users/ASUS/Documents/Primodilas/docs/01_Foundations/10_Time_Framework.md) — Microsecond clocks, speed scaling, and history snaps.
* [11_Space_Framework](file:///C:/Users/ASUS/Documents/Primodilas/docs/01_Foundations/11_Space_Framework.md) — Infinite 2D plane topology and territory boundaries.
* [12_Energy_Framework](file:///C:/Users/ASUS/Documents/Primodilas/docs/01_Foundations/12_Energy_Framework.md) — Generation, dissipation, storage, and efficiency metrics.
* [13_Matter_Framework](file:///C:/Users/ASUS/Documents/Primodilas/docs/01_Foundations/13_Matter_Framework.md) — Density, phase states, and resource compositions.
* [14_Information_Framework](file:///C:/Users/ASUS/Documents/Primodilas/docs/01_Foundations/14_Information_Framework.md) — Perception, memory decay, and Sentinel broadcasts.
* [15_Optimization_Framework](file:///C:/Users/ASUS/Documents/Primodilas/docs/01_Foundations/15_Optimization_Framework.md) — Fitness/Cost ratio and multi-level utility evaluation.
* [16_Emergence_Framework](file:///C:/Users/ASUS/Documents/Primodilas/docs/01_Foundations/16_Emergence_Framework.md) — Conditions, designed phenomena, and anti-patterns.
* [17_Complexity_Framework](file:///C:/Users/ASUS/Documents/Primodilas/docs/01_Foundations/17_Complexity_Framework.md) — Complexity classes, feedback loops, and scalability tiers.
* [18_Deterministic_Simulation](file:///C:/Users/ASUS/Documents/Primodilas/docs/01_Foundations/18_Deterministic_Simulation.md) — Non-determinism sources and snapshot hashing contracts.
* [19_System_Laws](file:///C:/Users/ASUS/Documents/Primodilas/docs/01_Foundations/19_System_Laws.md) — Seven invariant laws every system must respect.
* [20_Glossary](file:///C:/Users/ASUS/Documents/Primodilas/docs/01_Foundations/20_Glossary.md) — Canonical terminology definitions for the entire Bible.

---

## Volume II — Simulation Systems (World Mechanics) ✅
*Specifies how subsystems process environment, genetics, weather, and kinematic interactions.*

* [01_Entity_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/02_Simulation/01_Entity_System.md) — 32-bit entity recycling, generational checks, and archetypes.
* [02_Component_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/02_Simulation/02_Component_System.md) — Contiguous sparse set layouts for cache-friendly iterations.
* [03_World_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/02_Simulation/03_World_System.md) — Central world loop ticks scheduling registry.
* [04_Environment_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/02_Simulation/04_Environment_System.md) — Sparse abiotic cells, biome structures, and resource spawns.
* [05_Climate_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/02_Simulation/05_Climate_System.md) — Seasonal cycles and regional heat propagation equations.
* [06_Weather_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/02_Simulation/06_Weather_System.md) — Stochastic wind forces, blizzards, and precipitation.
* [07_Ecosystem_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/02_Simulation/07_Ecosystem_System.md) — Lotka-Volterra dynamics, food chains, and biomass cycles.
* [08_Resource_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/02_Simulation/08_Resource_System.md) — Depletion/regeneration limits and mineral scarcity dynamics.
* [09_Production_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/02_Simulation/09_Production_System.md) — Fabrication stages, recipes format, and throughput modifiers.
* [10_Logistics_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/02_Simulation/10_Logistics_System.md) — Delaunay route grids, cargo tasks, and road speeds.
* [11_Population_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/02_Simulation/11_Population_System.md) — Gompertz mortality hazards, carrying caps, and migration steps.
* [12_Reproduction_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/02_Simulation/12_Reproduction_System.md) — Fission thresholds, sexual fusion compatibilities, and offsets placement.
* [13_Genetics_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/02_Simulation/13_Genetics_System.md) — 12-gene genome layouts, phenotype maps, and epistasis.
* [14_Evolution_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/02_Simulation/14_Evolution_System.md) — Continuous selection pressure, speciation boundaries, and drift.
* [15_Mutation_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/02_Simulation/15_Mutation_System.md) — Point shifts, macro vertex changes, and phylogenetic logs.
* [16_Life_Cycle_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/02_Simulation/16_Life_Cycle_System.md) — Growth radius curves, aging decays, and decay resources spawning.
* [17_Physics_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/02_Simulation/17_Physics_System.md) — Kinematics forces accumulation, speed limits, and move costs.
* [18_Collision_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/02_Simulation/18_Collision_System.md) — Broadphase grids, SAT projections, and Baumgarte separations.
* [19_Interaction_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/02_Simulation/19_Interaction_System.md) — Proximity consumptions, influence aura maps, and combat formulas.
* [20_Event_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/02_Simulation/20_Event_System.md) — Decoupled event bus structs and metrics collectors.

---

## Volume III — Intelligence & Civilization (Emergent Behavior) ✅
*Specifies cognitive mind structures, dynamic careers, and Sentinel nerves.*

* [01_Cognitive_Framework](file:///C:/Users/ASUS/Documents/Primodilas/docs/03_Intelligence/01_Cognitive_Framework.md) — The unified perception-to-action pipeline.
* [02_Perception_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/03_Intelligence/02_Perception_System.md) — Grid sensory queries, visual fields, and raycast occlusions.
* [03_Memory_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/03_Intelligence/03_Memory_System.md) — Ring buffers, coordinate logs, and exponential decay models.
* [04_Attention_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/03_Intelligence/04_Attention_System.md) — Proximity and urgency saliency filtering.
* [05_Emotion_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/03_Intelligence/05_Emotion_System.md) — Fear, rage, trust, and wonder decay variables.
* [06_Sentiment_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/03_Intelligence/06_Sentiment_System.md) — Directed relation weights and faction reputation values.
* [07_Personality_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/03_Intelligence/07_Personality_System.md) — DNA behavior gene modifiers and trait variances.
* [08_Motivation_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/03_Intelligence/08_Motivation_System.md) — Homeostasis hunger need curve values.
* [09_Goal_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/03_Intelligence/09_Goal_System.md) — Intention stacks and preemption threat checks.
* [10_Decision_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/03_Intelligence/10_Decision_System.md) — Utility AI math equations and anti-oscillation timers.
* [11_Behavior_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/03_Intelligence/11_Behavior_System.md) — Steering kinematics vectors and obstacle raycast reflections.
* [12_Learning_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/03_Intelligence/12_Learning_System.md) — Active task skill accumulation rates and mentoring.
* [13_Knowledge_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/03_Intelligence/13_Knowledge_System.md) — Technology databases and Sentinel map syncs.
* [14_Communication_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/03_Intelligence/14_Communication_System.md) — Alarm calls and drop package congestion thresholds.
* [15_Role_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/03_Intelligence/15_Role_System.md) — Shape vertex affinities and promotion rank checkmarks.
* [16_Profession_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/03_Intelligence/16_Profession_System.md) — Gatherer, builder, soldier, and scholar speed modifiers.
* [17_Skill_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/03_Intelligence/17_Skill_System.md) — Inactive skills decays and absolute caps limits.
* [18_Society_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/03_Intelligence/18_Society_System.md) — Core colony formation dynamics and social cohesion parameters.
* [19_Culture_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/03_Intelligence/19_Culture_System.md) — Horizontal, vertical, and oblique tech tradition transmissions.
* [20_Government_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/03_Intelligence/20_Government_System.md) — Sentinel policy modes and civic alerts rules.
* [21_Economy_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/03_Intelligence/21_Economy_System.md) — Supply-demand scarcity pricing curves.
* [22_Diplomacy_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/03_Intelligence/22_Diplomacy_System.md) — Allied, neutral, hostile treaties, and war flags.
* [23_Military_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/03_Intelligence/23_Military_System.md) — Squad wedge flockings and road patrol intercepts.
* [24_Research_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/03_Intelligence/24_Research_System.md) — Science scan rewards and experimental inputs.
* [25_Technology_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/03_Intelligence/25_Technology_System.md) — Metallurgy, stone roads, and drag coefficient tech trees.
* [26_Civilization_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/03_Intelligence/26_Civilization_System.md) — Storage depots, workshops, and defensive towers.
* [27_Sentinel_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/03_Intelligence/27_Sentinel_System.md) — Command radius variables and database coordinate syncing.
* [28_Collective_Intelligence](file:///C:/Users/ASUS/Documents/Primodilas/docs/03_Intelligence/28_Collective_Intelligence.md) — Distributed memory maps and trail reinforcement algorithms.

---

## Volume IV — Engine & Development (Implementation Guide) ✅
*Specifies allocators, sparse sets, event polling, rendering, and replay serialization.*

* [00_Engine_DNA](file:///C:/Users/ASUS/Documents/Primodilas/docs/04_Engine/00_Engine_DNA.md) — Universal design language, lifecycles, and concurrency contracts (Meta-Architecture).
* [00_Engine_Architecture_Blueprint](file:///C:/Users/ASUS/Documents/Primodilas/docs/04_Engine/00_Engine_Architecture_Blueprint.md) — Core engine philosophy, data flow, memory model, and coding standards.
* [01_Architecture](file:///C:/Users/ASUS/Documents/Primodilas/docs/04_Engine/01_Architecture.md) — Layered boundaries, platform wrappers, and synchronizations.
* [02_Module_Architecture](file:///C:/Users/ASUS/Documents/Primodilas/docs/04_Engine/02_Module_Architecture.md) — ShapeEngine static library targets and CMake definitions.
* [03_ECS_Architecture](file:///C:/Users/ASUS/Documents/Primodilas/docs/04_Engine/03_ECS_Architecture.md) — Sparse set dense indexes and deferred CommandBuffers.
* [04_Memory_Architecture](file:///C:/Users/ASUS/Documents/Primodilas/docs/04_Engine/04_Memory_Architecture.md) — Arena bump allocators and component memory pool parameters.
* [05_Rendering_Architecture](file:///C:/Users/ASUS/Documents/Primodilas/docs/04_Engine/05_Rendering_Architecture.md) — Drawing virtual APIs and alpha ticks interpolations.
* [06_AI_Architecture](file:///C:/Users/ASUS/Documents/Primodilas/docs/04_Engine/06_AI_Architecture.md) — Linear query arrays and allocation-free intention stacks.
* [07_Simulation_Architecture](file:///C:/Users/ASUS/Documents/Primodilas/docs/04_Engine/07_Simulation_Architecture.md) — Inputs, update physics, and write deferred tick phases.
* [08_Networking_Architecture](file:///C:/Users/ASUS/Documents/Primodilas/docs/04_Engine/08_Networking_Architecture.md) — Lockstep packets synchronization and state desync CRC32 hashes.
* [09_Save_Load_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/04_Engine/09_Save_Load_System.md) — Binary file schemas, endianness conversions, and versioning.
* [10_Replay_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/04_Engine/10_Replay_System.md) — Seed inputs coordinates logging and keyframe seeks.
* [11_Editor_Architecture](file:///C:/Users/ASUS/Documents/Primodilas/docs/04_Engine/11_Editor_Architecture.md) — Entity inspector overlays and sandbox shape editors.
* [12_Debugging_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/04_Engine/12_Debugging_System.md) — Lock-free log systems and invariant macro assertions.
* [13_Testing_Framework](file:///C:/Users/ASUS/Documents/Primodilas/docs/04_Engine/13_Testing_Framework.md) — Catch2 unit tests layouts and mock coordinates verification.
* [14_Validation_Framework](file:///C:/Users/ASUS/Documents/Primodilas/docs/04_Engine/14_Validation_Framework.md) — Physical laws conservations checks and fuzzing scripts.
* [15_Benchmark_Framework](file:///C:/Users/ASUS/Documents/Primodilas/docs/04_Engine/15_Benchmark_Framework.md) — Performance timing counters and cache lines miss profiling.
* [16_Optimization_Framework](file:///C:/Users/ASUS/Documents/Primodilas/docs/04_Engine/16_Optimization_Framework.md) — DOD cache rules, thread pools, and release config flags.
* [17_File_Structure](file:///C:/Users/ASUS/Documents/Primodilas/docs/04_Engine/17_File_Structure.md) — Source directory file hierarchy maps.
* [18_Coding_Standards](file:///C:/Users/ASUS/Documents/Primodilas/docs/04_Engine/18_Coding_Standards.md) — PascalCase naming guidelines and header inclusion rules.
* [19_Development_Workflow](file:///C:/Users/ASUS/Documents/Primodilas/docs/04_Engine/19_Development_Workflow.md) — CMake configuration and compilation instructions.
* [20_Project_Roadmap](file:///C:/Users/ASUS/Documents/Primodilas/docs/04_Engine/20_Project_Roadmap.md) — Completed milestones log and stage 4/5 roadmap.
* [21_Technology_Stack](file:///C:/Users/ASUS/Documents/Primodilas/docs/04_Engine/21_Technology_Stack.md) — Definitive technology stack constraints and repository folder maps.

---

## Volume V — Client, Game & Scenarios (Gameplay Reference) ✅
*Specifies GDD details, UI layouts, scenario configurations, and modding schemas.*

* [01_Game_Design_Document](file:///C:/Users/ASUS/Documents/Primodilas/docs/05_Client/01_Game_Design_Document.md) — Core loops, player deity actions, and difficulties.
* [02_Polygonal_Primordials_Vision](file:///C:/Users/ASUS/Documents/Primodilas/docs/05_Client/02_Polygonal_Primordials_Vision.md) — Narrative backgrounds and vector glowing styles.
* [03_User_Interface_Architecture](file:///C:/Users/ASUS/Documents/Primodilas/docs/05_Client/03_User_Interface_Architecture.md) — UI decoupled layouts and panels configurations.
* [04_Camera_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/05_Client/04_Camera_System.md) — Coordinate zoom mappings and tracking lerps.
* [05_Input_Handling_System](file:///C:/Users/ASUS/Documents/Primodilas/docs/05_Client/05_Input_Handling_System.md) — Unprojection equations and click event bindings.
* [06_Audio_System_Design](file:///C:/Users/ASUS/Documents/Primodilas/docs/05_Client/06_Audio_System_Design.md) — Spatial volume distance scaling and ambient triggers.
* [07_Visual_Aesthetics_Guide](file:///C:/Users/ASUS/Documents/Primodilas/docs/05_Client/07_Visual_Aesthetics_Guide.md) — HSL color pallets and particle spark specs.
* [08_Scenario_System_Architecture](file:///C:/Users/ASUS/Documents/Primodilas/docs/05_Client/08_Scenario_System_Architecture.md) — Starting configs JSON templates and constraints.
* [09_Scenario_Sandbox_ALife](file:///C:/Users/ASUS/Documents/Primodilas/docs/05_Client/09_Scenario_Sandbox_ALife.md) — Herbivore-predator starting oscillations and diversity logs.
* [10_Scenario_Civilization_War](file:///C:/Users/ASUS/Documents/Primodilas/docs/05_Client/10_Scenario_Civilization_War.md) — Faction starting coordinates and victory conditions.
* [11_Scenario_Ice_Age_Survival](file:///C:/Users/ASUS/Documents/Primodilas/docs/05_Client/11_Scenario_Ice_Age_Survival.md) — Global temperature exponential decays and survival criteria.
* [12_Scenario_Economic_Logistics](file:///C:/Users/ASUS/Documents/Primodilas/docs/05_Client/12_Scenario_Economic_Logistics.md) — Resource sector divisions and trade throughput.
* [13_Scenario_Modding_Extensions](file:///C:/Users/ASUS/Documents/Primodilas/docs/05_Client/13_Scenario_Modding_Extensions.md) — Custom shape JSON extensions and mod folders layout.
* [14_Procedural_World_Generation](file:///C:/Users/ASUS/Documents/Primodilas/docs/05_Client/14_Procedural_World_Generation.md) — Perlin noise terrains and Whittaker biomes maps.
* [15_Asset_Management_Pipeline](file:///C:/Users/ASUS/Documents/Primodilas/docs/05_Client/15_Asset_Management_Pipeline.md) — Font glyph caching and shader compilers fallback.
* [16_Diagnostics_Dev_Overlay](file:///C:/Users/ASUS/Documents/Primodilas/docs/05_Client/16_Diagnostics_Dev_Overlay.md) — Loop timing telemetry and density heatmaps.
* [17_Replay_Spectator_Interface](file:///C:/Users/ASUS/Documents/Primodilas/docs/05_Client/17_Replay_Spectator_Interface.md) — Timeline scrubbers and free camera specs.
* [18_Tutorial_Workflow](file:///C:/Users/ASUS/Documents/Primodilas/docs/05_Client/18_Tutorial_Workflow.md) — guided tutorial levels and learning goals.
* [19_Build_Release_Distribution](file:///C:/Users/ASUS/Documents/Primodilas/docs/05_Client/19_Build_Release_Distribution.md) — CPACK bundles layout and compiler release flags.
* [20_Post_Launch_Live_Ops](file:///C:/Users/ASUS/Documents/Primodilas/docs/05_Client/20_Post_Launch_Live_Ops.md) — Client diagnostics telemetry and hotfix patch procedures.

---

## Volume VI — Framework Architectures (Ecosystem References) ✅
*Specifies the 11 integrated frameworks combining engine modules into cohesive ecosystems following the 13-point design blueprint.*

* [01_Physics_Framework](file:///C:/Users/ASUS/Documents/Primodilas/docs/07_Frameworks/01_Physics_Framework.md) — Universal kinematics and collision resolution solvers.
* [02_Biology_Framework](file:///C:/Users/ASUS/Documents/Primodilas/docs/07_Frameworks/02_Biology_Framework.md) — Dynamic lifecycles and metabolic decay.
* [03_Evolution_Framework](file:///C:/Users/ASUS/Documents/Primodilas/docs/07_Frameworks/03_Evolution_Framework.md) — Offspring crossover and Gaussian mutations.
* [04_Intelligence_Framework](file:///C:/Users/ASUS/Documents/Primodilas/docs/07_Frameworks/04_Intelligence_Framework.md) — Perception sweeps and Utility AI motivations.
* [05_Civilization_Framework](file:///C:/Users/ASUS/Documents/Primodilas/docs/07_Frameworks/05_Civilization_Framework.md) — Storage depots and Sentinel networks.
* [06_Economy_Framework](file:///C:/Users/ASUS/Documents/Primodilas/docs/07_Frameworks/06_Economy_Framework.md) — Supply-demand price curves and logistics routing.
* [07_Environment_Framework](file:///C:/Users/ASUS/Documents/Primodilas/docs/07_Frameworks/07_Environment_Framework.md) — Perlin noise terrains and Whittaker biomes.
* [08_Rendering_Framework](file:///C:/Users/ASUS/Documents/Primodilas/docs/07_Frameworks/08_Rendering_Framework.md) — Decoupled render graphs and extraction buffers.
* [09_Resource_Framework](file:///C:/Users/ASUS/Documents/Primodilas/docs/07_Frameworks/09_Resource_Framework.md) — Importer caches and fonts loading.
* [10_Networking_Framework](file:///C:/Users/ASUS/Documents/Primodilas/docs/07_Frameworks/10_Networking_Framework.md) — Lockstep packets synchronization and rollback re-simulations.
* [11_Editor_Framework](file:///C:/Users/ASUS/Documents/Primodilas/docs/07_Frameworks/11_Editor_Framework.md) — Inspector panels, dev sandbox control widgets, and diagnostics overlays.

