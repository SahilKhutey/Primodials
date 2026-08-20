import { Camera, Clapperboard, Film, X } from "lucide-react";
import type { PresentationMode } from "@/hooks/usePresentationMode";

type Props = {
  mode: PresentationMode;
  onModeChange: (mode: PresentationMode) => void;
  onClose: () => void;
};

export function PresentationChrome({ mode, onModeChange, onClose }: Props) {
  return (
    <div className="fixed bottom-5 left-1/2 z-[70] -translate-x-1/2">
      <div className="flex items-center gap-1 rounded-2xl border border-white/10 bg-black/55 p-1.5 shadow-2xl backdrop-blur-xl">
        {([
          ["normal", Camera, "Normal mode"],
          ["cinematic", Film, "Cinematic mode"],
          ["capture", Clapperboard, "Capture mode"],
        ] as const).map(([id, Icon, label]) => (
          <button
            key={id}
            type="button"
            onClick={() => onModeChange(id)}
            title={label}
            aria-label={label}
            className={`rounded-xl px-3 py-2 transition ${
              mode === id
                ? id === "cinematic"
                  ? "bg-violet-400/15 text-violet-200 ring-1 ring-violet-300/20"
                  : id === "capture"
                    ? "bg-cyan-400/15 text-cyan-200 ring-1 ring-cyan-300/20"
                    : "bg-white/10 text-white"
                : "text-neutral-500 hover:bg-white/5 hover:text-neutral-200"
            }`}
          >
            <Icon size={14} />
          </button>
        ))}
        <div className="mx-1 h-5 w-px bg-white/10" />
        <button
          type="button"
          onClick={onClose}
          title="Close presentation controls"
          aria-label="Close presentation controls"
          className="rounded-xl p-2 text-neutral-500 hover:bg-white/5 hover:text-white"
        >
          <X size={14} />
        </button>
      </div>
    </div>
  );
}
