import { StrictMode } from 'react';
import { createRoot } from 'react-dom/client';
import './index.css';
import '@/styles/phase5.css';
import App from './App';
import { WallpaperRoot } from './WallpaperRoot';
import { AppErrorBoundary } from './components/AppErrorBoundary';

const params = new URLSearchParams(window.location.search);
const isWallpaperEngine = typeof (window as unknown as { wallpaperPropertyListener?: unknown }).wallpaperPropertyListener !== 'undefined';
const isWallpaperMode = isWallpaperEngine || params.get('wallpaper') === '1';

const container = document.getElementById('root');
if (container) {
  createRoot(container).render(
    <StrictMode>
      <AppErrorBoundary>
        {isWallpaperMode ? <WallpaperRoot /> : <App />}
      </AppErrorBoundary>
    </StrictMode>,
  );
}
