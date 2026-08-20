import { Dna } from "lucide-react";

type Props = { visible: boolean; message?: string };

export function LoadingScreen({
  visible,
  message = "Initializing the ecosystem…",
}: Props) {
  if (!visible) return null;
  return (
    <div className="fixed inset-0 z-[120] grid place-items-center bg-neutral-950">
      <div className="flex flex-col items-center text-center">
        <div className="relative mb-5">
          <div className="absolute -inset-4 rounded-3xl bg-cyan-400/10 blur-xl" />
          <div className="relative rounded-2xl border border-white/10 bg-white/[0.03] p-4 shadow-2xl">
            <Dna size={28} className="animate-pulse text-cyan-300" />
          </div>
        </div>
        <div className="text-[10px] font-semibold uppercase tracking-[0.25em] text-cyan-300/70">
          Polygonal Primordials
        </div>
        <div className="mt-2 text-sm text-neutral-400">{message}</div>
        <div className="mt-5 h-1 w-48 overflow-hidden rounded-full bg-white/5">
          <div className="h-full w-1/3 animate-[pp-loader_1.4s_ease-in-out_infinite] rounded-full bg-cyan-300/70" />
        </div>
      </div>
    </div>
  );
}
