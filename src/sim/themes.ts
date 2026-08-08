// Visual theme system for the wallpaper engine.
// Each theme defines a complete aesthetic: background gradient,
// creature color palette, ambient particle style, vignette, and
// pacing parameters.

export type AmbientParticleStyle = 'bubbles' | 'motes' | 'sparks' | 'snow' | 'pollen' | 'none';

export type PacingPreset = 'peaceful' | 'lively' | 'dynamic';

export type WallpaperTheme = {
  id: string;
  displayName: string;
  description: string;

  // Background gradient (top-to-bottom radial)
  bgInner: string;
  bgMid: string;
  bgOuter: string;

  // Creature palette — hues that organisms are tinted toward
  // Each entry is [hue, saturation, lightness]
  creaturePalette: [number, number, number][];

  // Ambient particle layer
  particleStyle: AmbientParticleStyle;
  particleCount: number;
  particleHueRange: [number, number]; // [minHue, maxHue]
  particleSizeRange: [number, number];

  // Vignette
  vignetteStrength: number; // 0–1

  // Glow intensity for creatures (multiplier on shadowBlur)
  glowIntensity: number; // 0.5–2.0

  // Pacing defaults for this theme
  defaultPacing: PacingPreset;
  speedMultiplier: number; // sim step speed
  maxPopulation: number;
  foodRate: number;
};

export const THEMES: WallpaperTheme[] = [
  {
    id: 'deep-sea',
    displayName: 'Deep Sea',
    description: 'Bioluminescent creatures drifting through abyssal darkness',
    bgInner: '#0a1628',
    bgMid: '#06101e',
    bgOuter: '#020812',
    creaturePalette: [
      [180, 80, 60], [200, 75, 55], [280, 70, 60],
      [320, 65, 55], [160, 70, 50], [190, 85, 65],
    ],
    particleStyle: 'bubbles',
    particleCount: 35,
    particleHueRange: [170, 210],
    particleSizeRange: [0.8, 2.5],
    vignetteStrength: 0.55,
    glowIntensity: 1.4,
    defaultPacing: 'peaceful',
    speedMultiplier: 0.6,
    maxPopulation: 250,
    foodRate: 1.5,
  },
  {
    id: 'coral-reef',
    displayName: 'Coral Reef',
    description: 'Vibrant tropical life in turquoise waters',
    bgInner: '#0d2e3e',
    bgMid: '#0a2533',
    bgOuter: '#061a26',
    creaturePalette: [
      [350, 75, 60], [20, 80, 60], [180, 70, 55],
      [45, 85, 60], [330, 70, 55], [10, 75, 58],
    ],
    particleStyle: 'bubbles',
    particleCount: 50,
    particleHueRange: [175, 200],
    particleSizeRange: [1.0, 3.0],
    vignetteStrength: 0.35,
    glowIntensity: 1.1,
    defaultPacing: 'lively',
    speedMultiplier: 0.8,
    maxPopulation: 350,
    foodRate: 2.5,
  },
  {
    id: 'aurora',
    displayName: 'Aurora',
    description: 'Translucent beings beneath shifting northern lights',
    bgInner: '#0a1a2a',
    bgMid: '#081524',
    bgOuter: '#040a14',
    creaturePalette: [
      [140, 60, 60], [180, 55, 58], [280, 50, 55],
      [100, 50, 55], [200, 60, 60], [260, 45, 55],
    ],
    particleStyle: 'motes',
    particleCount: 45,
    particleHueRange: [120, 280],
    particleSizeRange: [0.5, 2.0],
    vignetteStrength: 0.5,
    glowIntensity: 1.3,
    defaultPacing: 'peaceful',
    speedMultiplier: 0.5,
    maxPopulation: 200,
    foodRate: 1.2,
  },
  {
    id: 'microorganisms',
    displayName: 'Microorganisms',
    description: 'Glowing microbes under a microscope lens',
    bgInner: '#080808',
    bgMid: '#050505',
    bgOuter: '#020202',
    creaturePalette: [
      [120, 90, 55], [180, 85, 50], [280, 80, 50],
      [60, 85, 50], [320, 75, 50], [200, 90, 55],
    ],
    particleStyle: 'sparks',
    particleCount: 30,
    particleHueRange: [80, 320],
    particleSizeRange: [0.5, 1.5],
    vignetteStrength: 0.7,
    glowIntensity: 1.6,
    defaultPacing: 'lively',
    speedMultiplier: 0.7,
    maxPopulation: 300,
    foodRate: 2.0,
  },
  {
    id: 'forest-floor',
    displayName: 'Forest Floor',
    description: 'Earth-toned life in dappled woodland light',
    bgInner: '#1a1a0e',
    bgMid: '#12120a',
    bgOuter: '#080804',
    creaturePalette: [
      [80, 60, 45], [60, 55, 40], [40, 65, 45],
      [100, 50, 40], [30, 60, 42], [70, 55, 48],
    ],
    particleStyle: 'pollen',
    particleCount: 40,
    particleHueRange: [40, 80],
    particleSizeRange: [0.5, 2.0],
    vignetteStrength: 0.45,
    glowIntensity: 0.9,
    defaultPacing: 'peaceful',
    speedMultiplier: 0.55,
    maxPopulation: 220,
    foodRate: 1.8,
  },
  {
    id: 'crystal-cave',
    displayName: 'Crystal Cave',
    description: 'Crystalline organisms in a luminous cavern',
    bgInner: '#1a0a2a',
    bgMid: '#120820',
    bgOuter: '#080412',
    creaturePalette: [
      [280, 65, 55], [260, 60, 50], [300, 60, 55],
      [240, 55, 52], [320, 55, 50], [270, 70, 58],
    ],
    particleStyle: 'sparks',
    particleCount: 35,
    particleHueRange: [240, 320],
    particleSizeRange: [0.5, 1.8],
    vignetteStrength: 0.5,
    glowIntensity: 1.5,
    defaultPacing: 'lively',
    speedMultiplier: 0.65,
    maxPopulation: 250,
    foodRate: 1.6,
  },
  {
    id: 'cyberpunk',
    displayName: 'Cyberpunk',
    description: 'Neon cyan & magenta entities drifting in a dark synth grid matrix',
    bgInner: '#0c021a',
    bgMid: '#06010e',
    bgOuter: '#020005',
    creaturePalette: [
      [180, 100, 55], [300, 100, 55], [195, 95, 60],
      [320, 90, 60], [160, 85, 50], [280, 95, 55],
    ],
    particleStyle: 'sparks',
    particleCount: 45,
    particleHueRange: [180, 310],
    particleSizeRange: [0.8, 2.2],
    vignetteStrength: 0.6,
    glowIntensity: 1.8,
    defaultPacing: 'dynamic',
    speedMultiplier: 0.85,
    maxPopulation: 300,
    foodRate: 2.0,
  },
  {
    id: 'solar-flare',
    displayName: 'Solar Flare',
    description: 'Golden & crimson plasma life in deep stellar space',
    bgInner: '#2a0a04',
    bgMid: '#1c0502',
    bgOuter: '#0a0100',
    creaturePalette: [
      [35, 100, 55], [15, 95, 55], [45, 90, 60],
      [5, 85, 50], [55, 95, 60], [25, 90, 52],
    ],
    particleStyle: 'motes',
    particleCount: 40,
    particleHueRange: [10, 50],
    particleSizeRange: [0.8, 2.5],
    vignetteStrength: 0.5,
    glowIntensity: 1.6,
    defaultPacing: 'lively',
    speedMultiplier: 0.7,
    maxPopulation: 280,
    foodRate: 1.8,
  },
];

export const DEFAULT_THEME_ID = 'deep-sea';

export function getTheme(id: string): WallpaperTheme {
  return THEMES.find((t) => t.id === id) ?? THEMES[0];
}

// Pacing presets adjust simulation aggressiveness and speed
export const PACING_PRESETS: Record<PacingPreset, {
  label: string;
  description: string;
  speedMultiplier: number;
  maxPopulationMod: number;
  foodRateMod: number;
  aggressionMod: number;
  mutationMod: number;
}> = {
  peaceful: {
    label: 'Peaceful',
    description: 'Very slow, stable populations, minimal conflict',
    speedMultiplier: 0.5,
    maxPopulationMod: 0.8,
    foodRateMod: 1.3,
    aggressionMod: 0.5,
    mutationMod: 0.7,
  },
  lively: {
    label: 'Lively',
    description: 'Medium pace with occasional events and activity',
    speedMultiplier: 0.8,
    maxPopulationMod: 1.0,
    foodRateMod: 1.0,
    aggressionMod: 1.0,
    mutationMod: 1.0,
  },
  dynamic: {
    label: 'Dynamic',
    description: 'Faster evolution, more chaos and competition',
    speedMultiplier: 1.2,
    maxPopulationMod: 1.3,
    foodRateMod: 0.8,
    aggressionMod: 1.5,
    mutationMod: 1.3,
  },
};

// Tint a creature's hue toward the theme palette.
// Returns a new hue value blended toward the nearest palette entry.
export function tintHue(originalHue: number, theme: WallpaperTheme): number {
  // Find the nearest palette hue
  let nearest = theme.creaturePalette[0][0];
  let minDist = Infinity;
  for (const [h] of theme.creaturePalette) {
    let dist = Math.abs(originalHue - h);
    if (dist > 180) dist = 360 - dist;
    if (dist < minDist) {
      minDist = dist;
      nearest = h;
    }
  }
  // Blend 30% toward the palette hue
  let blended = originalHue * 0.7 + nearest * 0.3;
  if (blended < 0) blended += 360;
  if (blended >= 360) blended -= 360;
  return blended;
}
