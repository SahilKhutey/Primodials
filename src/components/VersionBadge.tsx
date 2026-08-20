import { PRODUCT_CHANNEL, PRODUCT_VERSION } from "@/lib/version";

export function VersionBadge({ visible = import.meta.env.DEV }: { visible?: boolean }) {
  if (!visible) return null;
  return (
    <div className="pointer-events-none fixed bottom-3 left-3 z-[80] rounded-lg border border-white/5 bg-black/30 px-2 py-1 font-mono text-[9px] text-white/30 backdrop-blur-sm">
      {PRODUCT_VERSION} · {PRODUCT_CHANNEL}
    </div>
  );
}
