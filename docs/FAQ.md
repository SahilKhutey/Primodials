# Frequently Asked Questions (FAQ)

---

## ❓ General Questions

### Is Polygonal Primordials free?
Yes! The game and live wallpaper engine are free (Pay What You Want) on itch.io and open source under the MIT License on GitHub.

### Can I run it as my desktop background?
Yes! Launch `PolygonalPrimordialsWallpaper.exe` to run the live wallpaper mode. It integrates into the Windows system tray and WorkerW desktop layer.

### What are the heritable traits?
Speed, Size, Vision Radius, Aggression, Defense, Metabolism, Reproduction Rate, and Lifespan.

---

## 💻 Technical Questions

### How does it handle 250,000 entities?
Using an archetype-based Entity Component System (`ECS2`), uniform grid spatial hash broadphase, and OpenGL instanced draw calls.
