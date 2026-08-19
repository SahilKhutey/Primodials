# 🔄 PRIMODIALS — Rendering Fallback Strategy

## 🎯 3-TIER PROGRESSIVE ENHANCEMENT ARCHITECTURE

```
┌──────────────────────────────────────────────────────────┐
│                Tier 1: WebGPU (Optimal)                  │
│ • 1,000,000+ entities @ 60 FPS                           │
│ • Full compute shader physics & instanced draw           │
│ • < 5% CPU utilization, negligible thermal load          │
├──────────────────────────────────────────────────────────┤
│                Tier 2: WebGL2 (Intermediate)             │
│ • 100,000 entities @ 60 FPS                              │
│ • GPU transform feedback physics with instancing         │
│ • ~25% CPU utilization                                   │
├──────────────────────────────────────────────────────────┤
│                Tier 3: Canvas 2D (Universal)             │
│ • 30,000 entities @ 30–60 FPS                            │
│ • CPU spatial hash + 2D path rendering                   │
│ • High CPU utilization (~80%)                            │
└──────────────────────────────────────────────────────────┘
```

---

## 🔍 DETECTION ALGORITHM

```typescript
export async function selectBestRenderer(): Promise<'webgpu' | 'webgl2' | 'canvas2d'> {
  if (typeof navigator !== 'undefined' && 'gpu' in navigator && navigator.gpu) {
    try {
      const adapter = await navigator.gpu.requestAdapter({ powerPreference: 'high-performance' });
      if (adapter) return 'webgpu';
    } catch {
      // Fall through to Tier 2
    }
  }

  try {
    const canvas = document.createElement('canvas');
    if (canvas.getContext('webgl2')) return 'webgl2';
  } catch {
    // Fall through to Tier 3
  }

  return 'canvas2d';
}
```
