# 📺 PRIMODIALS — YouTube Devlog #1 Script

**Title:** "Building a Real-Time Artificial Life Simulator in C++23 & React | Devlog #1"  
**Duration:** ~5 minutes  
**Target Channels:** YouTube, Hacker News, Reddit  

---

## 📜 OUTLINE & TALKING POINTS

### 0:00 – 0:30 | The Cold Open & Hook
- **Visual:** Smooth capture of high-density Algae Bloom biome running at 60 FPS.
- **Host (Sahil Khutey):** *"Most desktop wallpapers are either static images or looping video files that eat your CPU. For the last six months, I've been building Primordials — a living digital terrarium where thousands of creatures hunt, reproduce, mutate, and die in real-time."*

### 0:30 – 1:30 | The Ecology Mathematics
- **Visual:** LaTeX overlay displaying Lotka-Volterra equations alongside real population graphs.
- **Host:** *"The foundation is built on continuous Lotka-Volterra differential equations: $\frac{dx}{dt} = \alpha x - \beta x y$. It's a natural mathematical oscillator, but when you add spatial hashing, food density gradients, and genetic mutations, genuine emergence begins to happen."*

### 1:30 – 3:00 | The C++23 Engine & Web Architecture
- **Visual:** VS Code tour showing ECS structure, Catch2 test runner (221 tests passing), and Canvas 2D renderer.
- **Host:** *"To keep it running effortlessly in the background, we built a custom data-oriented ECS in modern C++23, alongside a lightweight React 18 Canvas 2D web client. By batching render calls and using uniform spatial partitioning, neighbor searches execute in $O(1)$ time, maintaining 60 FPS locked with less than 1.5% CPU overhead."*

### 3:00 – 4:15 | Emergence Demo & Custom Biomes
- **Visual:** Live interaction — injecting predators into an overpopulated zone and watching the population crash and rebound over two minutes.
- **Host:** *"Notice how the predators over-hunt, suffer starvation, and the prey recovers. You don't script this outcome — you simply define the laws of the biome, and the ecosystem balances itself."*

### 4:15 – 5:00 | Open Source & Community Call to Action
- **Visual:** GitHub repository, itch.io page, and Steam Wallpaper Engine Workshop.
- **Host:** *"Primordials is 100% open source under the MIT license. You can download the desktop app for free on itch.io, try the browser demo, or grab it on Wallpaper Engine. All links are in the description below. If you'd like to design your own biome, check out our JSON schema on GitHub!"*
