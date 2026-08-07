# Polygonal Primordials

An artificial life and evolution sandbox. Watch digital organisms eat, mutate, form colonies, build structures, and evolve neural brains in real time — all in your browser.

## What's Built

The **live web application** is the working product. It runs entirely in the browser with no backend required:

- **Real-time canvas simulation** — Hundreds of polygonal organisms rendered as colored shapes with pan/zoom camera, movement trails, and biome backgrounds
- **Emergent evolution** — Organisms reproduce, mutate, compete, and speciate. Traits like intelligence, build skill, curiosity, and adaptability evolve over generations via a neural brain system
- **Ecosystem dynamics** — Predator-prey relationships, herbivore/carnivore diets, resource scarcity, spatial clustering, altruism, dominance hierarchies, and niche breadth
- **Microbial layer** — Biofilm formation, quorum sensing, chemotaxis, sporulation, horizontal gene transfer (HGT), photosynthesis, symbiosis, parasitism, and disease
- **Social behavior** — Colony formation, structure building (nests, caches, workshops), knowledge discovery, alpha hierarchies, and competition events
- **Live data panels** — Real-time stats (population, species count, fitness, births/deaths/kills), species rankings with tier labels, entity inspector (click any organism to see its genome and traits), and an evolution timeline chart plotting 26+ metrics over time
- **Experiment system** — Save and load simulation snapshots to Supabase, replay experiments, compare runs
- **Full simulation controls** — Pause, step, speed control (0.25x–4x), reset, and live-editable parameters (mutation rate, food density, world size, chemical fields, boundary mode, and more)

## Tech Stack

- **Frontend**: React 18 + TypeScript + Vite
- **Styling**: Tailwind CSS
- **Icons**: Lucide React
- **Backend**: Supabase (experiment save/load, snapshot storage)
- **Simulation**: Custom TypeScript engine — deterministic RNG (SplitMix64/Xoshiro256), spatial hash grid for neighbor queries, neural-network-driven behavior, genome decoding, chemical diffusion field

## Getting Started

```bash
npm install
npm run dev
```

The app runs at `http://localhost:5173`.

To build for production:

```bash
npm run build
```

## How It Works

Each organism has a genome that encodes its traits — size, speed, diet, sense range, intelligence, build skill, curiosity, adaptability, and more. Organisms wander, forage, hunt, reproduce (with crossover + mutation), and die. Over thousands of ticks, species emerge, compete, go extinct, and evolve.

The simulation uses a spatial hash grid for O(1) neighbor queries, a deterministic RNG for reproducibility, and an optional chemical diffusion field that models nutrient gradients in the environment.

## C++ Engine (Architecture Scaffolding)

The repository also contains `Engine/` — a C++23 architecture design for a high-performance game engine with archetype ECS, custom allocators, deterministic math, lockstep netcode, and Lua scripting. This is **design scaffolding and documentation**, not a shipping product. It is not built or run as part of the web application.

## License

Proprietary — see [LICENSE](LICENSE). All rights reserved by the author.

## Architecture Guidelines

- **Determinism**: The simulation uses deterministic RNG (SplitMix64/Xoshiro256). The C++ engine design mandates `Shape::Math` instead of `<cmath>` for cross-platform bit-exact determinism.
- **Memory**: The C++ engine design uses custom allocators (Linear, Pool, Arena, Frame) to avoid heap fragmentation at scale.
- **Testing**: The C++ engine has a test suite in `EngineTests/`. The web app is verified via production builds.
