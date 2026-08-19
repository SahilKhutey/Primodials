# 🔤 PRIMODIALS — Typography System

---

## FONT STACK

### Primary: Space Grotesk
- **Source:** Google Fonts (SIL OFL 1.1)
- **Weights:** 400 (Regular), 500 (Medium), 700 (Bold), 900 (Black)
- **Usage:** Headings, branding, primary interactive controls
- **Fallbacks:** Inter, system-ui, sans-serif

### Monospace: JetBrains Mono
- **Source:** Google Fonts (SIL OFL 1.1)
- **Weights:** 400 (Regular), 500 (Medium), 700 (Bold)
- **Usage:** Simulation telemetry, coordinate systems, math formulas, code blocks
- **Fallbacks:** ui-monospace, Menlo, Consolas, monospace

---

## TYPE SCALE (Base = 16px)

| Token | rem | px | Usage |
| :--- | :--- | :--- | :--- |
| `text-xs` | 0.75rem | 12px | Captions, footnotes, badge labels |
| `text-sm` | 0.875rem | 14px | Secondary UI, telemetry, metadata |
| `text-base` | 1.0rem | 16px | Standard body copy |
| `text-lg` | 1.125rem | 18px | Lead paragraphs, callouts |
| `text-xl` | 1.25rem | 20px | Sub-card headers, section titles |
| `text-2xl` | 1.5rem | 24px | H3 section headings |
| `text-3xl` | 1.875rem | 30px | H2 page titles |
| `text-4xl` | 2.25rem | 36px | H1 main headlines |
| `text-5xl` | 3.0rem | 48px | Mobile hero title |
| `text-6xl` | 3.75rem | 60px | Desktop hero title |

---

## CSS IMPLEMENTATION

```css
@import url('https://fonts.googleapis.com/css2?family=Space+Grotesk:wght@400;500;700;900&family=JetBrains+Mono:wght@400;500;700&display=swap');

:root {
  --font-sans: 'Space Grotesk', Inter, system-ui, sans-serif;
  --font-mono: 'JetBrains Mono', ui-monospace, Menlo, monospace;
  --tracking-display: -0.02em;
  --tracking-tight: -0.015em;
  --leading-tight: 1.1;
  --leading-relaxed: 1.6;
}

body {
  font-family: var(--font-sans);
  font-size: 1rem;
  line-height: var(--leading-relaxed);
  color: var(--color-text-primary);
  background: var(--color-bg-deep);
  -webkit-font-smoothing: antialiased;
}

.font-display {
  font-family: var(--font-sans);
  font-weight: 900;
  letter-spacing: var(--tracking-display);
  line-height: 0.95;
}

.font-mono {
  font-family: var(--font-mono);
}
```
