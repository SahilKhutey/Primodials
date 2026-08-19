import { cn } from '@/lib/cn';

interface LogoProps {
  size?: number;
  variant?: 'primary' | 'mono-light' | 'mono-dark';
  className?: string;
}

export function Logo({ size = 64, variant = 'primary', className }: LogoProps) {
  const colors = {
    primary: { bg: '#0F3D3E', prey: '#F4F1DE', predator: '#FF4FA3' },
    'mono-light': { bg: 'transparent', prey: '#F4F1DE', predator: '#F4F1DE' },
    'mono-dark': { bg: 'transparent', prey: '#0A0908', predator: '#0A0908' },
  }[variant];

  return (
    <svg
      xmlns="http://www.w3.org/2000/svg"
      viewBox="0 0 256 256"
      width={size}
      height={size}
      className={cn('flex-shrink-0', className)}
      role="img"
      aria-label="Primordials logo"
    >
      {colors.bg !== 'transparent' && (
        <rect width="256" height="256" rx="48" fill={colors.bg} />
      )}
      <ellipse
        cx="128"
        cy="128"
        rx="84"
        ry="36"
        transform="rotate(-22 128 128)"
        fill="none"
        stroke={colors.prey}
        strokeWidth="2"
        strokeOpacity="0.4"
        strokeDasharray="4 6"
      />
      <circle cx="128" cy="128" r="26" fill={colors.prey} />
      <circle cx="128" cy="128" r="6" fill={colors.bg === 'transparent' ? '#F4F1DE' : colors.bg} />
      <polygon points="208,128 230,140 230,116" fill={colors.predator} />
    </svg>
  );
}
