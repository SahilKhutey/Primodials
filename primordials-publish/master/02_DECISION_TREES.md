# 🌳 PRIMODIALS — Decision Trees

Quick decision frameworks for critical situations.

---

## 🌳 TREE 1: Community Comment Triage

```
Comment Received
 │
 ├── Constructive Feedback? ──► Respond thoughtfully with technical context & timeline.
 │
 ├── Bug Report?            ──► "Thank you! Tracking this in GitHub Issues as #XYZ."
 │
 ├── Toxic / Harassment?    ──► Apply Moderation Playbook (Level 2 Timeout or Level 5 Ban).
 │
 └── Biome Suggestion?      ──► "Great concept! Would love to see this in #biome-workshop."
```

---

## 🌳 TREE 2: Biome PR Acceptance Decision

```
Biome PR Submitted
 │
 ├── Valid JSON? (validate-biome.js) ──► NO  ──► Request syntax fixes.
 │                                   ──► YES ──► Continue
 │
 ├── Palette from Brand Tokens?      ──► NO  ──► Request color token alignment.
 │                                   ──► YES ──► Continue
 │
 ├── 3-8 Balanced Species?           ──► NO  ──► Request species simplification.
 │                                   ──► YES ──► Continue
 │
 └── 60s Simulation Stable?          ──► NO  ──► Request coefficient tuning.
                                     ──► YES ──► MERGE, Credit Author & Announce!
```

---

## 🌳 TREE 3: Feature Release Go / No-Go

```
Release Candidate
 │
 ├── All Unit Tests Passing? (Catch2 + Vitest) ──► NO  ──► Fix defects before building.
 │                                              ──► YES ──► Continue
 │
 ├── Maintains 60 FPS Locked @ 250k Entities?  ──► NO  ──► Optimize ECS memory layout.
 │                                              ──► YES ──► Continue
 │
 ├── Zero DRM / Open MIT Core Preserved?        ──► NO  ──► Refactor paywall out of core.
 │                                              ──► YES ──► SHIP RELEASE!
```
