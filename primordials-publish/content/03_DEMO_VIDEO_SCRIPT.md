# 🎥 PRIMODIALS — 30-Second Demo Video Script

**Title:** "Primordials — A Living Ecosystem on Your Desktop"  
**Duration:** 30.0 seconds  
**Resolution:** 1920 × 1080 @ 30 FPS  
**Format:** MP4 (H.264, AAC Stereo Audio)  
**File Size Target:** <40 MB  

---

## 📜 TIMELINE & SCRIPT BREAKDOWN

| Timestamp | Visual Content | Audio & Musical Cue |
| :--- | :--- | :--- |
| **0.0s – 1.5s** | Black background fades in; `primordials.` logomark appears in bone white (`#F4F1DE`) with glowing magenta dot. | Ambient synth swell begins softly. |
| **1.5s – 5.0s** | Cut to **Algae Bloom** establishing shot. Green nutrients drift as hundreds of circular grazers feed peacefully. | Warm organic chords emerge (-6 dB). |
| **5.0s – 8.5s** | Cut to **Deep Sea** biome. Bioluminescent violet anglerfish and squid navigate abyssal darkness. | Sub-bass drop with subtle electronic hum. |
| **8.5s – 12.0s** | Cut to **Primordial Soup**. Glowing hydrothermal vents erupt with protocell clusters. | Music tempo gently accelerates. |
| **12.0s – 16.0s** | Wide panoramic pull-back. Text overlay: *"A living ecosystem on your desktop."* | Full harmonic resolution. |
| **16.0s – 20.0s** | Quick rapid-fire montage: Tundra → Coral Reef → Volcanic Vent (1.3s each). | Dynamic, energetic cadence. |
| **20.0s – 24.0s** | Cut to **App Interface**: User adjusts mutation rate slider, population graph updates in real time. | Subtle click sound effects on UI adjustment. |
| **24.0s – 27.5s** | Cut to **Full Desktop Wallpaper Mode**: Clean monitor setup with living organisms moving seamlessly. | Ambient music reaches crescendo. |
| **27.5s – 30.0s** | Outro Card: Logomark + *"Free & Open Source"* + `primordials.dev` + GitHub URL. | Smooth fade to silence. |

---

## 🛠️ EXPORT SPECIFICATION

```bash
ffmpeg -i master_timeline.mov \
  -c:v libx264 -preset slow -crf 20 \
  -c:a aac -b:a 192k \
  -movflags +faststart \
  -pix_fmt yuv420p \
  demo-video--30s--master.mp4
```
