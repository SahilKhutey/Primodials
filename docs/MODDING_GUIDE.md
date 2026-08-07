# Modding Guide

Create custom themes and Lua mods for Polygonal Primordials.

---

## 🎨 Overview

Polygonal Primordials supports two types of mods:
1. **Themes** — Visual customization (color palettes, particle density, ambient audio).
2. **Lua Scripts** — Behavior modifications and event hooks.

---

## 🎨 Creating Themes

A theme is a folder inside `Content/Themes/` containing a `theme.json` configuration file:

```json
{
  "id": "my-custom-theme",
  "displayName": "My Custom Theme",
  "author": "Sahil Khutey",
  "backgroundTopColor": [0.02, 0.03, 0.08, 1.0],
  "backgroundBottomColor": [0.0, 0.01, 0.04, 1.0],
  "vignetteColor": [0.0, 0.0, 0.05, 0.8],
  "speciesPalette": [
    [0.3, 0.9, 1.0, 1.0],
    [1.0, 0.4, 0.7, 1.0]
  ],
  "particleDensity": 0.5,
  "ambientAudio": "audio/ocean-ambient.ogg"
}
```

---

## 📜 Creating Lua Mods

Lua mods live in `Content/Mods/MyMod/scripts/main.lua`:

```lua
engine.on_event("tick", function(tick)
    local entities = engine.get_entities()
    -- Custom simulation behavior
end)
```
