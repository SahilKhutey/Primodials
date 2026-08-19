# Primordials — Wallpaper Engine Workshop Submission

## 📋 PRE-SUBMISSION CHECKLIST
- [ ] Steam Client with Wallpaper Engine installed
- [ ] Preview Image: `preview.png` (1920 × 1080 PNG)
- [ ] Preview Video: `preview.webm` (15–30s VP9 WebM)
- [ ] Built bundle in `dist-wallpaper/` containing `index.html` and assets
- [ ] Workshop metadata and tags configured in `project.json`

---

## 🛠️ PACKAGING & LOCAL VERIFICATION

Run the automated packaging script from the root repository:

```bash
npm.cmd run package:wallpaper
```

This builds the optimized standalone web package into `dist-wallpaper/` and creates the ready-to-upload `dist-wallpaper-engine/` package with `project.json`.

---

## 🚀 UPLOAD STEPS

1. Open **Wallpaper Engine** → Click **Wallpaper Editor**.
2. Select **Create Wallpaper** → Choose **Web Wallpaper**.
3. Point to `dist-wallpaper-engine/index.html`.
4. Fill in:
   - **Title:** `Primordials — Living Ecosystem`
   - **Description:** Paste copy from `workshop-config.json`
   - **Preview image:** Select `preview.png`
   - **Tags:** `Scene`, `Nature`, `Science`, `Relaxing`, `60 FPS`
5. Click **Workshop** → **Publish to Workshop**.
6. Set Visibility to **Public** upon launch verification.
