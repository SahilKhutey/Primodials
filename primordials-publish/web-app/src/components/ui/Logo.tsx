interface LogoProps {
  size?: number;
}

export function Logo({ size = 64 }: LogoProps) {
  return (
    <svg
      xmlns="http://www.w3.org/2000/svg"
      viewBox="0 0 256 256"
      width={size}
      height={size}
      role="img"
      aria-label="Primordials logo"
    >
      <rect width="256" height="256" rx="48" fill="#0F3D3E" />
      <ellipse
        cx="128"
        cy="128"
        rx="84"
        ry="36"
        transform="rotate(-22 128 128)"
        fill="none"
        stroke="#F4F1DE"
        strokeWidth="2"
        strokeOpacity="0.4"
        strokeDasharray="4 6"
      />
      <circle cx="128" cy="128" r="26" fill="#F4F1DE" />
      <circle cx="128" cy="128" r="6" fill="#0F3D3E" />
      <polygon points="208,128 230,140 230,116" fill="#FF4FA3" />
    </svg>
  );
}
