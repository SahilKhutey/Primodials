import { type ReactNode } from 'react';

interface WallpaperFrameProps {
  children: ReactNode;
}

export function WallpaperFrame({ children }: WallpaperFrameProps) {
  return (
    <div className="fixed inset-0 overflow-hidden bg-bg-void">
      {children}
    </div>
  );
}
