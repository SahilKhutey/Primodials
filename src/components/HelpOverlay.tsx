import { X } from 'lucide-react';

type Props = {
  open: boolean;
  onClose: () => void;
};

const shortcuts = [
  ['Space / P', 'Play / pause'],
  ['N / .', 'Advance one simulation step when paused'],
  ['R', 'Reset the world'],
  ['D', 'Open / close the ecosystem diary'],
  ['H / ?', 'Open help'],
  ['Ctrl/Cmd + F', 'Fullscreen'],
];

export function HelpOverlay({ open, onClose }: Props) {
  if (!open) return null;

  return (
    <div
      className="fixed inset-0 z-[95] grid place-items-center bg-black/65 p-4 backdrop-blur-sm"
      role="dialog"
      aria-modal="true"
      aria-labelledby="primordials-help-title"
    >
      <div className="w-full max-w-2xl rounded-3xl border border-white/10 bg-neutral-950/95 p-6 shadow-2xl shadow-black/60">
        <div className="flex items-start justify-between gap-4">
          <div>
            <div className="text-[10px] font-semibold uppercase tracking-[0.2em] text-cyan-300/70">
              Help
            </div>
            <h2 id="primordials-help-title" className="mt-1 text-xl font-bold text-white">
              Control your ecosystem
            </h2>
            <p className="mt-2 text-sm leading-6 text-neutral-500">
              Most actions are also available from the visible controls in Simulation View and Wallpaper Mode.
            </p>
          </div>

          <button
            type="button"
            onClick={onClose}
            aria-label="Close help"
            className="rounded-xl bg-white/5 p-2 text-neutral-400 hover:bg-white/10 hover:text-white"
          >
            <X size={16} />
          </button>
        </div>

        <div className="mt-6 grid gap-2 sm:grid-cols-2">
          {shortcuts.map(([key, label]) => (
            <div
              key={key}
              className="flex items-center justify-between gap-3 rounded-2xl border border-white/5 bg-white/[0.03] px-4 py-3"
            >
              <span className="text-xs text-neutral-400">{label}</span>
              <kbd className="rounded-lg border border-white/10 bg-black/30 px-2.5 py-1.5 font-mono text-[10px] text-neutral-200">
                {key}
              </kbd>
            </div>
          ))}
        </div>

        <div className="mt-6 rounded-2xl border border-cyan-400/10 bg-cyan-400/[0.04] p-4 text-xs leading-5 text-neutral-400">
          <strong className="text-cyan-200">Tip:</strong> For the wallpaper build, keep quality on Medium first.
          Increase to High only after confirming your hardware remains comfortable during long runs.
        </div>
      </div>
    </div>
  );
}
