# Theme Creation Guide

Complete specification for Polygonal Primordials visual themes.

---

## 📁 Theme Directory Layout

```
Content/Themes/my-theme/
├── theme.json         # Main theme properties (required)
├── preview.png        # 256x256 thumbnail
├── title.png          # Title artwork
└── audio/
    └── ambient.ogg    # Ambient background track
```

---

## 🎨 Theme Fields

- `id`: Unique string identifier
- `displayName`: User-facing title
- `backgroundTopColor`: RGBA float array [0..1]
- `backgroundBottomColor`: RGBA float array [0..1]
- `vignetteIntensity`: Float 0..1
- `speciesPalette`: Array of RGBA color arrays for species visualization
- `ambientAudio`: Relative path to audio file
