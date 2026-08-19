# Polygonal Primordials — Phase 4 UX Specification

## Product promise

> A living artificial ecosystem on your desktop.

## UX principles

### 1. Observe first

The simulation should remain visually dominant.

### 2. Controls second

Controls appear when needed and recede when not needed.

### 3. Explain the consequence

Every advanced option should tell the user what it changes.

### 4. Safe defaults

A first-time user should not need technical knowledge.

### 5. Recovery over failure

A crash should lead to recovery, not a blank screen.

## Mode hierarchy

### Simulation View

Primary:
- ecosystem
- species
- inspector
- statistics

Secondary:
- history
- evolution
- settings

### Wallpaper Mode

Primary:
- ecosystem

Secondary:
- play/pause
- camera
- theme
- pacing
- settings

Transient:
- diary
- help
- share
- performance status

## First-run journey

1. Watch
2. Inspect
3. Shape
4. Share

No account requirement.

No cloud requirement.

No technical terminology during onboarding.

## Accessibility

Minimum:
- keyboard navigation for dialogs
- visible focus state
- reduced-motion respect
- buttons with `aria-label`
- meaningful dialog labels
- no interaction dependent on color alone

## Error handling

User-facing:
- simple explanation
- recovery instruction
- reload action

Developer-facing:
- stack trace in development builds only

## Store-demo discipline

For public screenshots/videos:
- first-run overlay hidden
- help hidden
- diagnostics hidden
- runtime HUD hidden
- no fake statistics
- actual gameplay only
