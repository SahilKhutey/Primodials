import type { Config } from 'tailwindcss';

export default {
  content: ['./index.html', './src/**/*.{ts,tsx}'],
  darkMode: 'class',
  theme: {
    extend: {
      colors: {
        bg: {
          deep: '#0F3D3E',
          void: '#0A0908',
          surface: '#1A4D4E',
          surfaceAlt: '#0E2E2F',
        },
        accent: {
          magenta: '#FF4FA3',
          coral: '#FF6B6B',
          amber: '#FFD93D',
          violet: '#9B5DE5',
        },
        text: {
          primary: '#F4F1DE',
          muted: '#A8B5A0',
          subtle: '#6F7A6B',
        },
        biome: {
          algae: '#4ECDC4',
          deepsea: '#FF6B6B',
          soup: '#FFD93D',
          tundra: '#6BCB77',
          coral: '#F18F01',
          volcanic: '#D62246',
        },
      },
      fontFamily: {
        sans: ['Inter', 'system-ui', 'sans-serif'],
        mono: ['JetBrains Mono', 'ui-monospace', 'Menlo', 'monospace'],
      },
    },
  },
  plugins: [],
} satisfies Config;
