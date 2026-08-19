# 🎞️ PRIMODIALS — 6-Second Demo GIF Storyboard

**Objective:** A seamless, hypnotic 6-second loop demonstrating real predator-prey dynamics to capture immediate interest.

**Specifications:**
- **Dimensions:** 1280 × 720 px
- **Frame Rate:** 30 FPS (180 frames total)
- **Duration:** 6.0 seconds exact
- **Target File Size:** <4.0 MB (optimized for Twitter & GitHub)
- **Looping:** Seamless loop matching frame 0 and frame 180

---

## 🎬 FRAME TIMELINE (180 Frames @ 30 FPS)

| Time | Frame | Action & Visual Composition | Text Overlay |
| :--- | :--- | :--- | :--- |
| **0.0s** | `0` | Deep Teal background fades up; establishing shot of Algae Bloom biome. | `primordials.` (bottom-left, 36px, 60% opacity) |
| **0.6s** | `18` | Slow forward camera drift begins (1% scale zoom over 6s). | — |
| **1.5s** | `45` | Predator triangle enters frame, locking onto a cluster of grazers. | — |
| **2.5s** | `75` | Grazers scatter via sensory evasion vectors; predator pursues. | — |
| **3.5s** | `105` | Camera pulls back slightly to reveal 200+ thriving background organisms. | — |
| **4.5s** | `135` | Full ecosystem equilibrium visible across screen. | *"A living ecosystem on your desktop."* (Center-bottom) |
| **5.5s** | `165` | Climax of movement; organisms drift gracefully. | Tagline holds |
| **5.8s** | `174` | Subtle cross-fade transition starts toward initial particle state. | Tagline fades |
| **6.0s** | `180` | Seamless return to Frame 0 coordinates. | Loop completes |

---

## 🛠️ CONVERSION & COMPRESSION PIPELINE

```bash
# Extract PNG frames and generate optimized palette
ffmpeg -i raw-capture.mp4 -ss 00:00:12 -t 6 -vf "fps=30,scale=1280:-1:flags=lanczos,split[s0][s1];[s0]palettegen=stats_mode=diff[p];[s1][p]paletteuse=dither=bayer:bayer_scale=5" -loop 0 demo-gif--algae-bloom--6s--twitter.gif

# Optional lossless compression pass with gifsicle
gifsicle -O3 --lossy=80 --colors 128 demo-gif--algae-bloom--6s--twitter.gif -o demo-gif--algae-bloom--6s--opt.gif
```
