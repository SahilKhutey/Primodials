# 🎞️ PRIMODIALS — Post-Production Editing & Grading Guide

---

## 🎨 COLOR GRADING WORKFLOW (DaVinci Resolve / Premiere Pro)

1. **Base Contrast & Lift:**
   - Lift black level slightly (`+0.02`) to ensure OLED backgrounds maintain depth without crushing organism silhouettes.
   - Boost highlights (`+0.05`) on cyan (`#4ECDC4`) and magenta (`#FF4FA3`) channels for bioluminescent pop.
2. **Subtle Vignette:**
   - 25% circular vignette on corners to draw the eye toward the center-screen biological interactions.
3. **Film Grain:**
   - 3% fine 35mm grain overlay to break up digital banding in gradient backgrounds.

---

## 🎬 EXPORT PROFILES (FFmpeg Commands)

### 1. High-Quality Web Video (1080p Master)
```bash
ffmpeg -i raw_capture.mp4 \
  -c:v libx264 -preset slow -crf 19 \
  -c:a aac -b:a 192k \
  -movflags +faststart \
  -pix_fmt yuv420p \
  demo-video--30s--master.mp4
```

### 2. Looping 6-Second GIF (<4MB)
```bash
ffmpeg -i raw_clip.mp4 -ss 00:00:10 -t 6 \
  -vf "fps=30,scale=1280:-1:flags=lanczos,split[s0][s1];[s0]palettegen=stats_mode=diff[p];[s1][p]paletteuse=dither=bayer:bayer_scale=5" \
  -loop 0 demo-gif--algae-bloom--6s--twitter.gif
```

### 3. Screenshot Batch Compression
```bash
# Convert all PNG masters into next-gen WebP formats
for f in *.png; do
  cwebp -q 85 "$f" -o "${f%.png}.webp"
done
```
