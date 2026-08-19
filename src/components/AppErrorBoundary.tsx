import React, { type ErrorInfo, type ReactNode } from 'react';
import { AlertTriangle, RefreshCcw } from 'lucide-react';

type Props = {
  children: ReactNode;
};

type State = {
  error: Error | null;
};

export class AppErrorBoundary extends React.Component<Props, State> {
  state: State = { error: null };

  static getDerivedStateFromError(error: Error): State {
    return { error };
  }

  componentDidCatch(error: Error, info: ErrorInfo) {
    if (import.meta.env.DEV) {
      console.error('Polygonal Primordials render error:', error, info);
    }
  }

  private handleReload = () => {
    window.location.reload();
  };

  render() {
    if (!this.state.error) return this.props.children;

    return (
      <div className="grid min-h-screen place-items-center bg-neutral-950 px-6 text-neutral-100">
        <div className="w-full max-w-lg rounded-3xl border border-red-400/15 bg-neutral-900/80 p-7 shadow-2xl shadow-black/50 backdrop-blur-xl">
          <div className="mb-4 flex h-11 w-11 items-center justify-center rounded-2xl bg-red-400/10 ring-1 ring-red-300/15">
            <AlertTriangle className="text-red-300" size={21} />
          </div>

          <div className="text-[10px] font-semibold uppercase tracking-[0.2em] text-red-300/70">
            Runtime recovery
          </div>

          <h1 className="mt-1 text-xl font-bold text-white">
            The interface hit an unexpected error.
          </h1>

          <p className="mt-3 text-sm leading-6 text-neutral-400">
            Your local recovery system may still contain the latest world state.
            Reload the application and use recovery when offered.
          </p>

          {import.meta.env.DEV && (
            <pre className="mt-4 max-h-40 overflow-auto rounded-xl bg-black/30 p-3 text-[10px] leading-4 text-red-200/80">
              {this.state.error.stack ?? this.state.error.message}
            </pre>
          )}

          <button
            type="button"
            onClick={this.handleReload}
            className="mt-6 flex items-center gap-2 rounded-xl bg-red-400/10 px-4 py-2.5 text-xs font-semibold text-red-200 ring-1 ring-red-300/15 hover:bg-red-400/15 transition"
          >
            <RefreshCcw size={14} />
            Reload application
          </button>
        </div>
      </div>
    );
  }
}
