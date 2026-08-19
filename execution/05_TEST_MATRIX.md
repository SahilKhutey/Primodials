# 🧪 Polygonal Primordials — Hardware Test Matrix

**Purpose:** Verify v1 works on real hardware before public launch.
**Rule:** Every claim in marketing must be backed by a passing test.

---

## 1. MINIMUM TEST COVERAGE

Before v1 launch, verify on AT LEAST these configurations:

### Hardware Tiers

| Tier | Specs |
|------|-------|
| LOW | Dual-core 2 GHz, 4 GB RAM, integrated GPU |
| MID | Quad-core 3 GHz, 8 GB RAM, mid-range GPU |
| HIGH | 8+ core, 16+ GB RAM, modern GPU |

### OS / Browser Matrix

| Platform | Required | Browser |
|----------|----------|---------|
| Windows 10 | ✅ | Chrome / Firefox / Edge |
| Windows 11 | ✅ | Chrome / Firefox / Edge |
| macOS 12+ | ✅ | Safari / Chrome |
| Ubuntu 22.04+ | ✅ | Chrome / Firefox |
| Steam Deck | ✅ | Steam client browser |

### Display Configurations

| Configuration | Required |
|---------------|----------|
| 1920×1080 | ✅ |
| 2560×1440 | ✅ |
| 3840×2160 (4K) | ✅ |
| Ultrawide 21:9 | ✅ |
| 3440×1440 | ✅ |
| Dual monitor | ✅ |
| Triple monitor | Recommended |

---

## 2. FUNCTIONAL TESTS

Each must pass on each hardware tier:

### Simulation
- [ ] Simulation runs continuously without crashes (24h test)
- [ ] FPS meets target (60 FPS LOW/MID, 30+ FPS HIGH)
- [ ] Population dynamics behave as expected
- [ ] Reproduction occurs at expected rates
- [ ] Death from age, hunger, predation works
- [ ] Population recovers from extinction

### Rendering
- [ ] All 6 biomes render correctly
- [ ] No visual artifacts (glitches, z-fighting, missing sprites)
- [ ] Color palette matches brand
- [ ] Animation is smooth (no jitter)

### UI
- [ ] Biome selector works
- [ ] Settings panel opens/closes
- [ ] Pause/resume button works
- [ ] Speed controls work
- [ ] Zoom in/out works
- [ ] Reset button works
- [ ] Save/load works

### Wallpaper Mode
- [ ] `?wallpaper=1` URL parameter activates wallpaper mode
- [ ] Runs behind desktop icons
- [ ] Doesn't steal focus
- [ ] Performance stays smooth
- [ ] Memory usage stable over time

---

## 3. STABILITY TESTS

### 24-Hour Test (all biomes)
For each biome:
1. Launch simulation
2. Leave running for 24 hours
3. Monitor:
   - FPS (should stay stable)
   - Memory (should not grow unbounded)
   - CPU usage (should not spike abnormally)
   - Crashes
   - Entity count (should oscillate, not zero out)
4. Record results

### Sleep / Wake Test
- [ ] System sleep → app pauses / doesn't consume CPU
- [ ] System wake → app resumes correctly
- [ ] Display sleep → app continues running (wallpaper mode)

### Resolution Change Test
- [ ] Change resolution mid-simulation → app adapts
- [ ] No crash
- [ ] No visual glitches

---

## 4. PERFORMANCE TEST RECORD

For each test run, record:

```yaml
date: 2026-XX-XX
hardware:
  tier: MID
  cpu: "Intel i5-10400"
  ram: 8 GB
  gpu: "GTX 1650"
  os: "Windows 11"
  browser: "Chrome 131"
biome: algae-bloom
entity_count: 800
results:
  fps_avg: 60
  fps_min: 58
  fps_p99: 60
  cpu_avg: 8%
  ram_growth_24h: 5 MB
  crashes: 0
  notes: "Stable, smooth"
```

---

## 5. WHAT TO DO WITH RESULTS

- **Pass:** Document, use as marketing claim
- **Fail:** Fix, retest
- **Marginal:** Lower setting (entity count, FPS target)

---

## 6. CLAIMS WE CAN MAKE AFTER TESTING

Only after hardware tests pass:

| Claim | Evidence required |
|-------|-------------------|
| "60 FPS on mid-range hardware" | Test on MID tier |
| "Works on Windows 10/11" | Test on Win 10 + Win 11 |
| "Works on macOS" | Test on macOS 12+ |
| "Works on Linux" | Test on Ubuntu 22.04+ |
| "Stable for 24+ hours" | 24h test on each biome |
| "Low CPU usage" | CPU measurement |
| "Works offline" | Airplane mode test |

---

## 7. CLAIMS WE MUST REMOVE (until tested)

- ❌ "250,000 entities @ 60 FPS" (unverified for web)
- ❌ "Multiplayer" (not in v1)
- ❌ "11 languages" (not in v1)
- ❌ "Vulkan support" (not relevant — web app uses Canvas 2D)
- ❌ "Steam Workshop" (not in v1)

---

## 8. TEST SCHEDULE

- **Week 1:** LOW tier + Windows tests
- **Week 2:** MID tier + macOS + Linux
- **Week 3:** HIGH tier + ultrawide + multi-monitor
- **Week 4:** 72-hour stability + bug fixes
- **Week 5:** Final QA + marketing claim verification

---

## 9. BUG TRACKING

Use GitHub Issues. Each bug report includes:
- Hardware tier
- Browser/OS
- Biome
- Steps to reproduce
- Expected vs actual
- Screenshots / video

*Critical bugs block launch. Major bugs must be fixed or documented. Minor bugs can be post-launch.*
