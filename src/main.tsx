import { StrictMode, Component, type ReactNode, type ErrorInfo } from 'react';
import { createRoot } from 'react-dom/client';
import './index.css';
import App from './App';
import { WallpaperRoot } from './WallpaperRoot';

// Error boundary so any crash renders visibly instead of a silent black screen
class ErrorBoundary extends Component<{ children: ReactNode }, { error: Error | null }> {
  state = { error: null };
  static getDerivedStateFromError(error: Error) { return { error }; }
  componentDidCatch(error: Error, info: ErrorInfo) {
    console.error('[ErrorBoundary] caught:', error, info.componentStack);
  }
  render() {
    if (this.state.error) {
      const e = this.state.error as Error;
      return (
        <div style={{ background: '#1a0000', color: '#ff6060', padding: 32, fontFamily: 'monospace', fontSize: 14 }}>
          <h2 style={{ color: '#ff3030', marginBottom: 16 }}>⚠ App crashed during initialization</h2>
          <p><b>{e.name}:</b> {e.message}</p>
          <pre style={{ marginTop: 16, color: '#ffaaaa', whiteSpace: 'pre-wrap', fontSize: 12 }}>{e.stack}</pre>
        </div>
      );
    }
    return this.props.children;
  }
}

const params = new URLSearchParams(window.location.search);
const isWallpaperEngine = typeof (window as unknown as { wallpaperPropertyListener?: unknown }).wallpaperPropertyListener !== 'undefined';
const isWallpaperMode = isWallpaperEngine || params.get('wallpaper') === '1';

const container = document.getElementById('root');
if (container) {
  createRoot(container).render(
    <StrictMode>
      <ErrorBoundary>
        {isWallpaperMode ? <WallpaperRoot /> : <App />}
      </ErrorBoundary>
    </StrictMode>,
  );
}
