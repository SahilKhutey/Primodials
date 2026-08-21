import { IS_STABLE, RELEASE_CHANNEL } from "@/lib/releaseChannel";

export function ReleaseChannelBadge() {
  if (IS_STABLE) return null;
  return (
    <div className="pointer-events-none fixed left-3 top-3 z-[100] rounded-lg border border-amber-300/10 bg-black/30 px-2 py-1 font-mono text-[9px] uppercase tracking-wider text-amber-200/50">
      {RELEASE_CHANNEL}
    </div>
  );
}
