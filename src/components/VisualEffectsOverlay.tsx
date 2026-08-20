import type { WallpaperTheme } from "@/sim/themes";

type Props = {
  theme: WallpaperTheme;
  cinematic?: boolean;
  captureMode?: boolean;
  reducedMotion?: boolean;
};

function rgbaFromHex(hex: string, alpha: number): string {
  const normalized = hex.replace("#", "");
  const full = normalized.length === 3
    ? normalized.split("").map((c) => c + c).join("")
    : normalized;
  const value = Number.parseInt(full, 16);
  return `rgba(${(value >> 16) & 255}, ${(value >> 8) & 255}, ${value & 255}, ${alpha})`;
}

export function VisualEffectsOverlay({
  theme,
  cinematic = false,
  captureMode = false,
  reducedMotion = false,
}: Props) {
  const [h, s, l] = theme.creaturePalette[0] ?? [190, 80, 60];
  const paletteColor = `hsl(${h}deg ${s}% ${l}%)`;

  return (
    <div className="pointer-events-none fixed inset-0 z-[5] overflow-hidden" aria-hidden="true">
      <div
        className={`absolute inset-0 transition-opacity duration-1000 ${cinematic ? "opacity-100" : "opacity-80"}`}
        style={{
          background: `
            radial-gradient(circle at 50% 45%, ${rgbaFromHex(theme.bgInner, 0.12)} 0%, transparent 42%),
            radial-gradient(circle at 18% 25%, ${paletteColor} 0%, transparent 34%)
          `,
          opacity: 0.12,
        }}
      />
      <div
        className={`absolute inset-0 ${captureMode ? "opacity-100" : "opacity-80"}`}
        style={{
          boxShadow: `inset 0 0 140px ${rgbaFromHex(theme.bgOuter, Math.min(0.95, theme.vignetteStrength))}`,
        }}
      />
      <div
        className="absolute inset-0 opacity-[0.035] mix-blend-screen"
        style={{
          backgroundImage:
            "linear-gradient(rgba(255,255,255,.08) 1px, transparent 1px), linear-gradient(90deg, rgba(255,255,255,.08) 1px, transparent 1px)",
          backgroundSize: "80px 80px",
          animation: reducedMotion ? undefined : "pp-grid-drift 28s linear infinite",
        }}
      />
      <div
        className="absolute inset-0 opacity-[0.05]"
        style={{ background: "radial-gradient(circle at center, transparent 0 58%, rgba(0,0,0,.85) 100%)" }}
      />
    </div>
  );
}
