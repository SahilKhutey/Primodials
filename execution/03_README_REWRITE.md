# 📝 Polygonal Primordials — README Rewrite (Consumer-First)

**Purpose:** Replace the current README with a consumer-facing version
that immediately answers: "What is this? Why should I care? How do I get it?"

---

## 1. CURRENT README PROBLEMS

The current README:
- ❌ Leads with C++23 engine talk
- ❌ Claims "MIT licensed" while LICENSE file says proprietary
- ❌ Describes complex multi-binary setup (Windows installer, Linux .deb, Linux AppImage, macOS .pkg)
- ❌ Includes features not yet verified (multiplayer, Steam Workshop, 11 languages, Vulkan)
- ❌ Buries the actual user-facing product under architecture details

---

## 2. NEW README STRUCTURE (consumer-first)

```markdown
# 🧬 Polygonal Primordials

**A living ecosystem on your desktop.**

[Trailer GIF]
[Watch on YouTube]  [Get on Wallpaper Engine]  [Download on itch.io]

Polygonal Primordials turns your desktop into a window into a tiny,
evolving artificial ecosystem. Watch predator and prey populations
rise and fall, see creatures adapt, leave it running while you work
— come back to a world that has changed.

> Built with care by a solo developer in Bilaspur, India.
> Free. Open source. MIT.

---

## 🌿 What is it?

A real-time simulation of an artificial ecosystem running on your
desktop. Organisms move, hunt, reproduce and die according to
ecological rules (predator-prey dynamics). The ecosystem you see is
genuinely alive — not a video loop.

---

## 🎯 What you can do

- 🧬 **Watch evolution happen** — creatures inherit traits
- 🌿 **Choose a biome** — six ecosystems included
- 🖥️ **Use it as wallpaper** — runs behind your desktop
- 🎮 **Interact** — pause, speed up, inspect creatures
- 📖 **Learn ecology** — see predator-prey dynamics in action
- 🛠️ **Make your own** — create custom biomes with JSON

---

## 📸 Screenshots

[Screenshot grid: 6 biomes]

---

## 🚀 Get it

### Option 1 — Wallpaper Engine (FREE, easiest)
[Install from Steam Workshop](link)
- One-click install
- Auto-updates
- Works on Steam Deck, Windows, Linux

### Option 2 — itch.io (FREE / pay what you want)
[Download on itch.io](link)
- Standalone app
- No Steam required
- All platforms

### Option 3 — Build from source
[Build instructions](docs/build.md)
- For developers
- Requires Node.js 18+

---

## 📋 System requirements

- **Minimum:** Dual-core 2 GHz, 2 GB RAM, OpenGL 3.3+ or modern browser
- **Recommended:** Quad-core, 4 GB RAM, dedicated GPU
- **OS:** Windows 10+, macOS 12+, Ubuntu 22.04+, modern browsers

---

## 🛠️ Tech stack

- React 18 + TypeScript
- Custom entity-component-system
- Canvas 2D rendering (WebGPU in development)
- Vite + Tailwind

The full technical architecture is in [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md).

---

## 📚 Documentation

- 📖 [Getting Started](docs/INSTALLATION.md)
- 🎮 [Controls](docs/CONTROLS.md)
- 🌿 [Biome Schema](docs/BIOME_SCHEMA.md)
- 🛠️ [Build Instructions](docs/BUILD.md)
- 🧬 [Architecture](docs/ARCHITECTURE.md)

---

## 🤝 Contributing

- 🌿 [Contribute a biome](docs/CONTRIBUTING_BIOMES.md)
- 🐛 [Report a bug](../../issues)
- 💡 [Suggest a feature](../../issues)
- 💻 [Submit code](CONTRIBUTING.md)

---

## 📜 License

This project is released under the **MIT License**.
See [LICENSE](LICENSE) for details.

---

## 💖 Acknowledgments

Built by **Sahil Khutey** (Gensouls Lab, Bilaspur, India).
Inspired by classical ecology mathematics.
```

---

## 3. WHAT TO REMOVE FROM CURRENT README

- ❌ "250,000+ entities @ 60 FPS" (verify or remove)
- ❌ "Lockstep networking" (not in v1)
- ❌ "Multiplayer" (not in v1)
- ❌ "Steam Workshop" (not in v1)
- ❌ "11 languages" (not in v1)
- ❌ "OpenGL 4.6" / "Vulkan" (not in v1 — web uses Canvas 2D)
- ❌ Detailed binary installation steps for Win/macOS/Linux installers (use Wallpaper Engine or itch.io instead)

---

## 4. WHAT TO KEEP

- ✅ Project description (rewritten)
- ✅ Features list (verified only)
- ✅ Installation (pointed at Wallpaper Engine + itch.io + build)
- ✅ Contributing
- ✅ License (after fix)
- ✅ Tech stack (briefly)

---

## 5. POSITIONING

The README should make a user feel:
- **Within 5 seconds:** "Oh, a desktop ecosystem wallpaper"
- **Within 15 seconds:** "This looks beautiful and I want it"
- **Within 30 seconds:** "Here's how to get it"
- **Within 60 seconds:** "I have it running and it works"

---

## 6. LINK STRATEGY

- `README.md` (this file) — what + why + how to get
- `docs/INSTALLATION.md` — detailed install
- `docs/CONTROLS.md` — how to use
- `docs/ARCHITECTURE.md` — for developers (separate)
- `CONTRIBUTING.md` — for contributors (separate)
- `docs/BIOME_SCHEMA.md` — for biome creators (separate)

*README is the FRONT DOOR. Everything else is a deeper room.*
