import { useEffect, useState } from "react";

export type WallpaperViewport = {
  width: number;
  height: number;
  aspect: number;
  ultrawide: boolean;
  portrait: boolean;
};

function getViewport(): WallpaperViewport {
  const width = Math.max(1, window.innerWidth);
  const height = Math.max(1, window.innerHeight);
  const aspect = width / height;

  return {
    width,
    height,
    aspect,
    ultrawide: aspect >= 2,
    portrait: aspect < 0.9,
  };
}

export function useResponsiveWallpaper(): WallpaperViewport {
  const [viewport, setViewport] = useState<WallpaperViewport>(() => getViewport());

  useEffect(() => {
    const update = () => setViewport(getViewport());
    window.addEventListener("resize", update, { passive: true });
    return () => window.removeEventListener("resize", update);
  }, []);

  return viewport;
}
