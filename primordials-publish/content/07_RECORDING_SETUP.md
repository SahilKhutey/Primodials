# 🎙️ PRIMODIALS — Video & Audio Recording Setup Guide

---

## 🖥️ OBS STUDIO CONFIGURATION

### Output Settings
- **Output Mode:** Advanced
- **Format:** MKV (lossless during unexpected crashes; remux to MP4 via OBS menu)
- **Video Encoder:** NVENC H.264 (NVIDIA) / AMF (AMD) / QuickSync (Intel) / x264 (CPU fallback)
- **Rate Control:** Constant Bitrate (CBR) @ `22,000 kbps`
- **Preset:** P6: Slower (Better Quality)
- **Tuning:** High Quality / Two Passes (Quarter Resolution)

### Video Settings
- **Base Canvas Resolution:** `1920 × 1080` (or `3840 × 2160` for 4K captures)
- **Output Scaled Resolution:** `1920 × 1080`
- **Downscale Filter:** Lanczos (Sharpened scaling, 36 samples)
- **Common FPS Values:** `60 FPS` locked

---

## 🎤 AUDIO CALIBRATION

- **Microphone Input:** -6 dB to -3 dB peak level (avoid clipping in 0 dB red zone).
- **Noise Suppression:** Speex / RNNoise plugin enabled in OBS filters.
- **Compressor Settings:** Threshold `-18 dB`, Ratio `3:1`, Attack `6 ms`, Release `60 ms`.

---

## 🧼 CLEAN RECORDING ENVIRONMENT CHECKLIST

1. **Desktop Prep:**
   - Hide desktop icons (Right-click desktop → *View* → *Show desktop icons* [Uncheck]).
   - Auto-hide taskbar / dock.
   - Set desktop wallpaper to solid Abyssal Teal (`#0F3D3E`).
2. **Notifications:**
   - Enable Windows Focus Assist (Priority Only) / macOS Do Not Disturb.
3. **App Execution:**
   - Launch simulation with clean deterministic seed: `npm run dev -- --wallpaper=1`.
   - Run 30 seconds before recording to allow initial populations to settle into harmonic equilibrium.
