# ⚡ PRIMODIALS — Daily Quick Reference Cheat Sheet

---

## 📅 DAILY OPERATIONAL CADENCE

```
Morning (9:00 AM, 30 min):
  □ Inspect overnight analytics & traffic spikes on Plausible.
  □ Triage incoming GitHub issues and PR submissions.
  □ Review Discord notifications & community #help tickets.

Midday (1:00 PM, 15 min):
  □ Respond to social discussions (Twitter / Reddit).
  □ Welcome new community members in Discord #introductions.

Evening (6:00 PM, 15 min):
  □ Run end-of-day telemetry sync.
  □ Queue next day's release candidate or dev log snippet.
```

---

## 🎨 BRAND PALETTE TOKENS

- **Primary Deep Teal:** `#0F3D3E` (Main background & base UI tone)
- **Primary Void Black:** `#0A0908` (Canvas void & contrast backing)
- **Electric Magenta Accent:** `#FF4FA3` (Key calls to action & highlights)
- **Bone White Text:** `#F4F1DE` (Primary typography & readable foreground)
- **Sage Muted Text:** `#A8B5A0` (Secondary labels & metadata)

---

## 🛠️ CRITICAL CLI SHORTCUTS

```bash
# Validate any custom biome JSON file
node scripts/validate-biome.js biomes/algae-bloom.json

# Run all Catch2 C++ unit tests
ctest --preset dev --output-on-failure

# Package all production storefront release ZIPs
npm.cmd run package:all
```
