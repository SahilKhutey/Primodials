# 🌐 PRIMODIALS — Favicon Specifications & Manifests

---

## 1. FILE ROSTER

| Filename | Dimensions | Format | Target Platform |
| :--- | :--- | :--- | :--- |
| `favicon.svg` | Scalable | SVG | Modern standard browsers |
| `favicon.ico` | 16, 32, 48 | ICO | Legacy browser fallbacks |
| `favicon-16x16.png` | 16×16 | PNG | Browser tab |
| `favicon-32x32.png` | 32×32 | PNG | Browser tab retina |
| `apple-touch-icon.png`| 180×180 | PNG | iOS Home Screen bookmark |
| `android-chrome-192x192.png` | 192×192 | PNG | Android PWA install |
| `android-chrome-512x512.png` | 512×512 | PNG | Android splash screen |
| `mstile-150x150.png` | 150×150 | PNG | Windows Start Menu tile |
| `safari-pinned-tab.svg`| Scalable | Monochrome SVG | Safari pinned tab bar |
| `site.webmanifest` | Text | JSON | PWA Manifest |
| `browserconfig.xml` | Text | XML | Windows tile manifest |

---

## 2. HTML HEAD EMBED CODE

```html
<link rel="icon" type="image/svg+xml" href="/favicon/favicon.svg">
<link rel="icon" type="image/png" sizes="32x32" href="/favicon/favicon-32x32.png">
<link rel="icon" type="image/png" sizes="16x16" href="/favicon/favicon-16x16.png">
<link rel="apple-touch-icon" sizes="180x180" href="/favicon/apple-touch-icon.png">
<link rel="manifest" href="/favicon/site.webmanifest">
<meta name="theme-color" content="#0F3D3E">
<meta name="msapplication-TileColor" content="#0F3D3E">
<link rel="mask-icon" href="/favicon/safari-pinned-tab.svg" color="#0F3D3E">
```
