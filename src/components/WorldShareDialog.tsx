import { useState } from "react";
import type { SimSettings } from "@/sim/types";
import { createWorldShare, decodeWorldShare, shareToUrl } from "@/lib/worldShare";

type Props = {
  seed: number;
  settings: SimSettings;
  onLoadShare: (seed: number, settings: Partial<SimSettings>) => void;
  onClose: () => void;
};

export function WorldShareDialog({
  seed,
  settings,
  onLoadShare,
  onClose,
}: Props) {
  const [mode, setMode] = useState<"share" | "load">("share");
  const [value, setValue] = useState(() =>
    createWorldShare(seed, {
      worldWidth: settings.worldWidth,
      worldHeight: settings.worldHeight,
      initialPopulation: settings.initialPopulation,
      initialFood: settings.initialFood,
      mutationRate: settings.mutationRate,
      maxPopulation: settings.maxPopulation,
      maxFood: settings.maxFood,
      boundaryMode: settings.boundaryMode,
      biomes: settings.biomes,
      advancedBiology: settings.advancedBiology,
      socialBehavior: settings.socialBehavior,
    }, "Primordials World"),
  );
  const [message, setMessage] = useState("");

  const copy = async () => {
    try {
      if (typeof navigator !== "undefined" && navigator.clipboard) {
        await navigator.clipboard.writeText(shareToUrl(value));
        setMessage("Share URL copied to clipboard.");
      } else {
        setMessage("Clipboard unavailable — copy the text manually.");
      }
    } catch {
      setMessage("Clipboard unavailable — copy the text manually.");
    }
  };

  const load = () => {
    const parsed = decodeWorldShare(value);
    if (!parsed) {
      setMessage("Invalid or unsupported world code.");
      return;
    }
    onLoadShare(parsed.seed, parsed.settings);
    onClose();
  };

  return (
    <div className="fixed inset-0 z-50 grid place-items-center bg-black/60 p-4 backdrop-blur-sm">
      <div className="w-full max-w-xl rounded-2xl border border-white/10 bg-neutral-950 p-5 shadow-2xl">
        <div className="mb-4 flex items-center justify-between">
          <div>
            <h2 className="text-base font-bold text-white">World Share</h2>
            <p className="text-xs text-neutral-500">Share a reproducible world seed and configuration.</p>
          </div>
          <button onClick={onClose} className="text-xs text-neutral-400 hover:text-white">Close</button>
        </div>

        <div className="mb-3 flex gap-2">
          <button
            onClick={() => setMode("share")}
            className={`rounded-lg px-3 py-1.5 text-xs font-medium transition-colors ${mode === "share" ? "bg-cyan-500/20 text-cyan-300 ring-1 ring-cyan-400/30" : "bg-neutral-900 text-neutral-400"}`}
          >
            Share
          </button>
          <button
            onClick={() => setMode("load")}
            className={`rounded-lg px-3 py-1.5 text-xs font-medium transition-colors ${mode === "load" ? "bg-cyan-500/20 text-cyan-300 ring-1 ring-cyan-400/30" : "bg-neutral-900 text-neutral-400"}`}
          >
            Load
          </button>
        </div>

        <textarea
          value={value}
          onChange={(e) => setValue(e.target.value)}
          className="min-h-32 w-full resize-y rounded-xl border border-white/10 bg-black/40 p-3 font-mono text-xs text-neutral-200 outline-none focus:border-cyan-400/40"
          spellCheck={false}
        />

        <div className="mt-3 flex items-center justify-between gap-3">
          <span className="text-xs text-neutral-400">{message}</span>
          {mode === "share" ? (
            <button
              onClick={copy}
              className="rounded-xl bg-cyan-500/20 px-4 py-2 text-xs font-semibold text-cyan-300 ring-1 ring-cyan-400/20 hover:bg-cyan-500/30 transition-colors"
            >
              Copy URL
            </button>
          ) : (
            <button
              onClick={load}
              className="rounded-xl bg-emerald-500/20 px-4 py-2 text-xs font-semibold text-emerald-300 ring-1 ring-emerald-400/20 hover:bg-emerald-500/30 transition-colors"
            >
              Load World
            </button>
          )}
        </div>
      </div>
    </div>
  );
}
