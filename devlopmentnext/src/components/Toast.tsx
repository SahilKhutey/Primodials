import { useEffect } from 'react';
import { CheckCircle2, Info, TriangleAlert, X } from 'lucide-react';

export type ToastKind = 'success' | 'info' | 'warning';

export type ToastItem = {
  id: number;
  message: string;
  kind: ToastKind;
};

type Props = {
  toast: ToastItem | null;
  onDismiss: () => void;
  durationMs?: number;
};

const iconByKind = {
  success: CheckCircle2,
  info: Info,
  warning: TriangleAlert,
};

export function Toast({ toast, onDismiss, durationMs = 3500 }: Props) {
  useEffect(() => {
    if (!toast) return;
    const id = window.setTimeout(onDismiss, durationMs);
    return () => window.clearTimeout(id);
  }, [toast, onDismiss, durationMs]);

  if (!toast) return null;

  const Icon = iconByKind[toast.kind];

  return (
    <div className="pointer-events-none fixed inset-x-4 top-4 z-[110] flex justify-center">
      <div className="pointer-events-auto flex max-w-xl items-center gap-3 rounded-2xl border border-white/10 bg-neutral-950/90 px-4 py-3 text-xs text-white shadow-2xl backdrop-blur-xl">
        <Icon
          size={16}
          className={
            toast.kind === 'success'
              ? 'text-emerald-300'
              : toast.kind === 'warning'
                ? 'text-amber-300'
                : 'text-cyan-300'
          }
        />
        <span className="flex-1 text-neutral-300">{toast.message}</span>
        <button
          type="button"
          onClick={onDismiss}
          aria-label="Dismiss notification"
          className="rounded-lg p-1 text-neutral-500 hover:bg-white/5 hover:text-white"
        >
          <X size={13} />
        </button>
      </div>
    </div>
  );
}
