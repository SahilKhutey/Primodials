import { Logo } from '@/components/ui/Logo';

export function LoadingScreen() {
  return (
    <div className="flex h-screen w-screen items-center justify-center bg-bg-void">
      <div className="flex flex-col items-center gap-4">
        <Logo size={80} />
        <div className="font-display text-2xl font-bold tracking-tight text-text-primary">
          primordials<span className="text-accent-magenta">.</span>
        </div>
        <div className="font-mono text-sm text-text-muted">
          initializing ecosystem...
        </div>
        <div className="h-1 w-32 overflow-hidden rounded-full bg-bg-surface">
          <div className="h-full w-1/3 animate-pulse bg-accent-magenta" />
        </div>
      </div>
    </div>
  );
}
