import { useMemo } from "react";
import { CheckCircle2, AlertCircle } from "lucide-react";
import { runReleasePreflight } from "@/lib/releasePreflight";

export function ReleasePreflightPanel({ visible }: { visible: boolean }) {
  const checks = useMemo(() => (visible ? runReleasePreflight() : []), [visible]);
  if (!visible) return null;
  const failed = checks.filter((check) => !check.ok).length;

  return (
    <div className="fixed right-4 top-4 z-[90] w-[min(360px,calc(100vw-2rem))] rounded-2xl border border-white/10 bg-black/60 p-4 font-mono shadow-2xl backdrop-blur-xl">
      <div className="mb-3">
        <div className="text-[9px] font-semibold uppercase tracking-[0.2em] text-white/35">Release preflight</div>
        <div className="mt-1 text-xs font-semibold text-white">{failed === 0 ? "PASS" : `${failed} warning(s)`}</div>
      </div>
      <div className="space-y-2">
        {checks.map((check) => (
          <div key={check.id} className="flex items-start gap-2">
            {check.ok
              ? <CheckCircle2 size={13} className="mt-0.5 shrink-0 text-emerald-300" />
              : <AlertCircle size={13} className="mt-0.5 shrink-0 text-amber-300" />}
            <div className="min-w-0">
              <div className="text-[10px] text-white/75">{check.label}</div>
              <div className="text-[9px] text-white/35">{check.detail}</div>
            </div>
          </div>
        ))}
      </div>
    </div>
  );
}
