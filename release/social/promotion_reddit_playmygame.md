Title: [Dev] Polygonal Primordials - Ecosystem simulation with real genetic algorithms (C++23, open source)

Hi r/playmygame! I'm a solo developer who built Polygonal Primordials - a data-oriented ecosystem simulation in C++23.

What it is:
A real-time simulation where you watch polygonal creatures with heritable traits evolve through actual genetic algorithms.

Features:
- Custom ECS handling 250K entities @ 60 FPS
- 8 heritable traits per creature (speed, size, vision, aggression, etc.)
- Watch emergent behavior: evolution, speciation, extinction
- Full parameter control (mutation rate, food spawn, predation)
- Live visualization with heatmaps and population graphs
- Open source engine under MIT license

Technical:
- Built from scratch in C++23 (no Unity, no Unreal)
- Custom archetype-based Entity-Component System
- Spatial hash for O(1) neighbor queries
- OpenGL 4.6 instanced rendering

Play free: https://gensoulslab.itch.io/polygonal-primordials
Source code: https://github.com/SahilKhutey/Primodials

Would love to hear your feedback on the simulation behavior and performance!
