/** @type {import('tailwindcss').Config} */
module.exports = {
  content: ['./src/**/*.{ts,tsx,md,mdx}'],
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
        sans: ['var(--font-sans)', 'Space Grotesk', 'Inter', 'system-ui', 'sans-serif'],
        mono: ['var(--font-mono)', 'JetBrains Mono', 'ui-monospace', 'Menlo', 'monospace'],
        display: ['var(--font-sans)', 'Space Grotesk', 'Inter', 'sans-serif'],
      },
      fontSize: {
        'display-1': ['6rem', { lineHeight: '0.95', letterSpacing: '-0.02em' }],
        'display-2': ['4.5rem', { lineHeight: '1.0', letterSpacing: '-0.015em' }],
        'display-3': ['3.75rem', { lineHeight: '1.1', letterSpacing: '-0.015em' }],
      },
      letterSpacing: {
        tightest: '-0.025em',
        tighter: '-0.015em',
        tight: '-0.01em',
      },
      animation: {
        'fade-in': 'fadeIn 0.8s ease-out',
        'fade-up': 'fadeUp 0.8s ease-out',
        'pulse-slow': 'pulse 4s ease-in-out infinite',
        'orbit-slow': 'orbit 16s linear infinite',
        'gradient-shift': 'gradientShift 8s ease-in-out infinite',
        'float': 'float 6s ease-in-out infinite',
      },
      keyframes: {
        fadeIn: {
          '0%': { opacity: '0' },
          '100%': { opacity: '1' },
        },
        fadeUp: {
          '0%': { opacity: '0', transform: 'translateY(20px)' },
          '100%': { opacity: '1', transform: 'translateY(0)' },
        },
        pulse: {
          '0%, 100%': { opacity: '1' },
          '50%': { opacity: '0.5' },
        },
        orbit: {
          '0%': { transform: 'rotate(0deg) translateX(60px) rotate(0deg)' },
          '100%': { transform: 'rotate(360deg) translateX(60px) rotate(-360deg)' },
        },
        gradientShift: {
          '0%, 100%': { backgroundPosition: '0% 50%' },
          '50%': { backgroundPosition: '100% 50%' },
        },
        float: {
          '0%, 100%': { transform: 'translateY(0)' },
          '50%': { transform: 'translateY(-10px)' },
        },
      },
      backgroundImage: {
        'gradient-hero': 'linear-gradient(135deg, #0A0908 0%, #0F3D3E 50%, #1A4D4E 100%)',
        'gradient-accent': 'linear-gradient(90deg, #FF4FA3 0%, #9B5DE5 100%)',
        'gradient-radial': 'radial-gradient(ellipse at center, #0F3D3E 0%, #0A0908 100%)',
        'dot-pattern': "radial-gradient(circle, rgba(244,241,222,0.06) 1px, transparent 1px)",
      },
      backgroundSize: {
        'dot-32': '32px 32px',
      },
    },
  },
  plugins: [
    require('@tailwindcss/forms'),
    require('@tailwindcss/typography'),
  ],
};
