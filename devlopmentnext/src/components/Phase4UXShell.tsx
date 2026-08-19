import { useCallback, useState } from 'react';
import { LifeBuoy } from 'lucide-react';
import { useFirstRun } from '@/hooks/useFirstRun';
import { useKeyboardShortcuts } from '@/hooks/useKeyboardShortcuts';
import { FirstRunOverlay } from '@/components/FirstRunOverlay';
import { HelpOverlay } from '@/components/HelpOverlay';
import { Toast, type ToastItem, type ToastKind } from '@/components/Toast';

type Props = {
  children: React.ReactNode;
  running: boolean;
  onToggleRun: () => void;
  onStep?: () => void;
  onReset?: () => void;
  onToggleFullscreen?: () => void;
  onToggleDiary?: () => void;
  showHelpButton?: boolean;
};

export function Phase4UXShell({
  children,
  running,
  onToggleRun,
  onStep,
  onReset,
  onToggleFullscreen,
  onToggleDiary,
  showHelpButton = true,
}: Props) {
  const firstRun = useFirstRun();
  const [helpOpen, setHelpOpen] = useState(false);
  const [toast, setToast] = useState<ToastItem | null>(null);

  const notify = useCallback(
    (message: string, kind: ToastKind = 'info') => {
      setToast({
        id: Date.now(),
        message,
        kind,
      });
    },
    [],
  );

  useKeyboardShortcuts(
    {
      onToggleRun,
      onStep,
      onReset,
      onToggleFullscreen,
      onToggleHelp: () => setHelpOpen((value) => !value),
      onToggleDiary,
    },
    !firstRun.open && !helpOpen,
  );

  return (
    <>
      {children}

      {showHelpButton && (
        <button
          type="button"
          onClick={() => setHelpOpen(true)}
          aria-label="Open help"
          title="Help (H or ?)"
          className="fixed right-4 bottom-4 z-40 flex h-10 w-10 items-center justify-center rounded-2xl border border-white/10 bg-neutral-950/65 text-neutral-400 shadow-xl backdrop-blur-xl transition hover:bg-neutral-900 hover:text-white"
        >
          <LifeBuoy size={16} />
        </button>
      )}

      <FirstRunOverlay
        open={firstRun.open}
        onComplete={() => {
          firstRun.complete();
          notify(
            running
              ? 'Welcome. The ecosystem is already running.'
              : 'Welcome. Start the ecosystem when you are ready.',
            'success',
          );
        }}
      />

      <HelpOverlay
        open={helpOpen}
        onClose={() => setHelpOpen(false)}
      />

      <Toast
        toast={toast}
        onDismiss={() => setToast(null)}
      />
    </>
  );
}
