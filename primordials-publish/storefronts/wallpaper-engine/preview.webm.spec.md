# Wallpaper Engine Preview Video Spec

**File:** `preview.webm`  
**Duration:** 15–30 seconds  
**Resolution:** 1920 × 1080  
**Format:** WebM (VP9 codec)  
**Frame rate:** 30 FPS  
**Loop:** Seamless  

---

## 🎬 SHOT TIMELINE

* **0:00 – 0:05:** Wide establishing shot — full Algae Bloom biome visible with high-density grazers.
* **0:05 – 0:10:** Slow camera drift into predator hunting event.
* **0:10 – 0:15:** Biome transition fade into Deep Sea bioluminescent creatures.
* **0:15 – 0:20:** Primordial Soup geothermal vent emergence.
* **0:20 – 0:25:** Smooth zoom out to show stable ecosystem carrying capacity.
* **0:25 – 0:30:** Return to default palette and seamless loop closure.

---

## 🔧 ENCODING COMMAND (FFmpeg)

```bash
ffmpeg -i raw_capture.mov \
  -c:v libvpx-vp9 -crf 30 -b:v 0 \
  -pix_fmt yuv420p \
  -deadline good \
  preview.webm
```
