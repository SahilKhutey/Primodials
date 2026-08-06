Title: [Show] Polygonal Primordials - Lessons from building a 250K-entity ECS in C++23

Body:
After a year of development, I'm sharing what I learned building Polygonal Primordials - a data-oriented ecosystem simulation.

Performance results:
- 100K entities: 60 FPS (mid-range GPU)
- 250K entities: 60 FPS (high-end GPU)
- 500K entities: 30 FPS (stress test)

Architecture highlights:
- Archetype-based ECS with chunk-based storage
- Spatial hash for O(1) neighbor queries  
- OpenGL instanced rendering (1 draw call for entire ecosystem)
- Custom memory allocators (pool, arena, linear)
- Deterministic simulation (lockstep multiplayer ready)

Full source: https://github.com/SahilKhutey/Primodials
Play free: https://gensoulslab.itch.io/polygonal-primordials

Happy to answer questions about ECS design, data-oriented programming, or any other technical details!
