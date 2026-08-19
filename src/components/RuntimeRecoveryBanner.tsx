type Props = {
  visible: boolean;
  onRecover: () => void;
  onDismiss: () => void;
};

export function RuntimeRecoveryBanner({
  visible,
  onRecover,
  onDismiss,
}: Props) {
  if (!visible) return null;

  return (
    <div className="fixed inset-x-4 bottom-4 z-50 mx-auto flex max-w-xl items-center gap-3 rounded-2xl border border-amber-400/20 bg-black/85 px-4 py-3 text-xs text-white shadow-2xl backdrop-blur-xl">
      <div className="flex-1">
        <div className="font-semibold text-amber-200">Previous session did not close cleanly.</div>
        <div className="mt-0.5 text-white/60">
          A local recovery point is available. Would you like to restore your simulation?
        </div>
      </div>
      <button
        onClick={onRecover}
        className="rounded-xl bg-amber-400/20 px-3 py-2 font-semibold text-amber-200 ring-1 ring-amber-300/30 hover:bg-amber-400/30 transition"
      >
        Recover
      </button>
      <button
        onClick={onDismiss}
        className="rounded-xl bg-white/10 px-3 py-2 text-white/60 hover:text-white transition"
      >
        Dismiss
      </button>
    </div>
  );
}
