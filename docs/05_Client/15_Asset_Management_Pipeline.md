# 15_Asset_Management_Pipeline

This document specifies the client asset caches, font loading parameters, and shader compilations.

---

## 1. Asset Cache Architecture
To prevent frame drops from runtime disk reads, all assets are cached in memory at startup:
* **Font Cache:** Pre-renders glyph sizes using TTF (TrueType Font) libraries.
* **Texture Cache:** Stores compiled sprites and UI icons (as SDL_Texture handles).

---

## 2. Font Configurations
The client utilizes Outfit and Outfit-Bold fonts for readability:
* Title displays use Outfit-Bold at size $24$.
* Detail panels use Outfit at size $14$ (highly readable, clean spacing).

---

## 3. Shader Compiler Pipeline
If the graphics backend is configured to use modern render APIs (OpenGL, Vulkan):
* Vertex and fragment shader code strings are compiled at startup.
* **Fallback:** If compilation fails (e.g. incompatible GPU drivers), the client falls back to the standard CPU rendering pipeline.
