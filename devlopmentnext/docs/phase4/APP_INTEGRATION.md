# Phase 4 — Actual Integration Steps

Phase 4 adds a commercial-product UX layer to the existing `src/App.tsx`.

The existing App already contains:
- `running`
- `handleReset`
- `handleStep`
- `toggleFullscreen`
- `showDiary`
- `setShowDiary`
- wallpaper mode
- sandbox mode

These are the handlers Phase 4 hooks into.

---

## Step 1 — Copy the new files

Copy these paths into the repository:

```text
src/hooks/useFirstRun.ts
src/hooks/useKeyboardShortcuts.ts

src/lib/uxCopy.ts
src/lib/uxAnnouncement.ts

src/components/FirstRunOverlay.tsx
src/components/HelpOverlay.tsx
src/components/Toast.tsx
src/components/AppErrorBoundary.tsx
src/components/Phase4UXShell.tsx
src/components/SettingsSection.tsx

tests/uxCopy.test.ts
```

---

## Step 2 — Wrap the App with the error boundary

Open your application entry point, usually:

```text
src/main.tsx
```

Change the render tree from:

```tsx
<StrictMode>
  <App />
</StrictMode>
```

to:

```tsx
<StrictMode>
  <AppErrorBoundary>
    <App />
  </AppErrorBoundary>
</StrictMode>
```

Import:

```ts
import { AppErrorBoundary } from '@/components/AppErrorBoundary';
```

Do NOT put `AppErrorBoundary` inside `App.tsx` itself.

The boundary must sit above the application.

---

## Step 3 — Integrate Phase4UXShell into App.tsx

At the top of `App.tsx` add:

```ts
import { Phase4UXShell } from '@/components/Phase4UXShell';
```

Then change the root return.

Current sandbox root begins with:

```tsx
return (
  <div className="min-h-screen bg-neutral-950 text-neutral-100 app-bg">
```

Instead use:

```tsx
return (
  <Phase4UXShell
    running={running}
    onToggleRun={() => setRunning((r) => !r)}
    onStep={handleStep}
    onReset={handleReset}
    onToggleFullscreen={toggleFullscreen}
    onToggleDiary={() => setShowDiary((value) => !value)}
  >
    <div className="min-h-screen bg-neutral-950 text-neutral-100 app-bg">
      {/* existing sandbox UI */}
    </div>
  </Phase4UXShell>
);
```

The shell owns:
- first-run onboarding
- Help button
- keyboard shortcuts
- Help overlay
- toasts

The existing UI remains the primary control surface.

---

## Step 4 — Integrate Phase4UXShell into wallpaper mode

The wallpaper branch currently returns a fixed full-screen container.

Wrap it:

```tsx
if (wallpaperMode) {
  return (
    <Phase4UXShell
      running={running}
      onToggleRun={() => setRunning((r) => !r)}
      onStep={handleStep}
      onReset={handleReset}
      onToggleFullscreen={toggleFullscreen}
      onToggleDiary={() => setShowDiary((value) => !value)}
    >
      <div className="fixed inset-0 overflow-hidden bg-black">
        {/* existing wallpaper UI */}
      </div>
    </Phase4UXShell>
  );
}
```

For the Wallpaper Engine-only build, you can use:

```tsx
showHelpButton={false}
```

if you want absolutely no additional persistent UI beyond the existing dock.

---

## Step 5 — Move the existing Space shortcut into the new shortcut system

App.tsx currently has its own Spacebar effect.

Remove the old block:

```ts
useEffect(() => {
  const onKey = (e: KeyboardEvent) => {
    ...
  };

  window.addEventListener('keydown', onKey);
  return () => window.removeEventListener('keydown', onKey);
}, []);
```

Otherwise Space will be handled twice.

Phase 4 now controls:
- Space / P
- N / .
- R
- D
- H / ?
- Ctrl/Cmd + F

---

## Step 6 — Add the Help button to the visual hierarchy

Do not add another large top navigation.

Phase 4 deliberately puts Help as:

```text
bottom-right
small
floating
```

This avoids competing with:
- the wallpaper dock
- diary button
- simulation panels
- selected organism inspector

---

## Step 7 — Add SettingsSection to the existing settings UI

Use it inside your existing settings panel.

Example:

```tsx
<SettingsSection
  title="Performance"
  description="Tune the visual workload without changing the simulation rules."
  tone="cyan"
>
  <SettingsRow
    label="Rendering Quality"
    description="Higher quality uses more rendering resources."
  >
    {/* existing quality buttons */}
  </SettingsRow>

  <SettingsRow
    label="Population Cap"
    description="Maximum active organism count."
  >
    {/* existing population slider */}
  </SettingsRow>
</SettingsSection>
```

Another useful section:

```tsx
<SettingsSection
  title="Accessibility"
  description="Reduce motion and non-essential visual effects."
  tone="violet"
>
  {/* accessibility settings */}
</SettingsSection>
```

---

## Step 8 — UX rules for the settings panel

Do not expose every engine parameter immediately.

Group settings as:

```text
Performance
├── Quality
├── Population
└── Visual effects

Simulation
├── Mutation
├── Biology
├── Neural systems
└── World expansion

Presentation
├── Theme
├── Pacing
└── Camera

Accessibility
├── Reduced motion
└── UI visibility
```

Advanced engine switches can remain inside a collapsed section.

---

## Step 9 — First-run behavior

First launch:

```text
Open app
 ↓
First Run Overlay
 ↓
Watch
 ↓
Inspect
 ↓
Shape
 ↓
Share
 ↓
Enter ecosystem
```

Subsequent launch:

```text
Open app
 ↓
Straight to ecosystem
```

This state is stored in:

```text
polygonal-primordials.first-run.v1
```

---

## Step 10 — QA for Phase 4

Verify:

```text
[ ] First-run overlay appears on fresh browser profile.
[ ] First-run overlay disappears permanently after completion.
[ ] Keyboard shortcuts work.
[ ] Inputs/textareas do not trigger shortcuts.
[ ] Help dialog opens with H.
[ ] Help dialog opens with ?.
[ ] Help closes normally.
[ ] Error boundary renders if a React child throws.
[ ] Reload from error screen works.
[ ] Settings remain understandable on narrow screens.
[ ] Existing wallpaper dock still works.
[ ] Diary shortcut does not interfere with typing.
[ ] Fullscreen shortcut works.
[ ] Space no longer toggles twice.
[ ] Reduced-motion behavior from Phase 3 remains intact.
```

---

## Step 11 — Verification commands

```bash
npm run typecheck
npm run lint
npm run test
npm run build
npm run build:wallpaper
npm run package:wallpaper
```

---

## Step 12 — Phase 4 exit criteria

Phase 4 is complete when:

```text
[ ] New users understand what Primordials is within 10 seconds.
[ ] New users can start without reading documentation.
[ ] Keyboard controls are discoverable.
[ ] Errors produce a recovery path.
[ ] Settings are organized by purpose.
[ ] Wallpaper mode remains visually clean.
[ ] Accessibility controls are discoverable.
[ ] Existing controls were not duplicated.
[ ] Existing simulation functionality remains unchanged.
[ ] All automated tests pass.
```
