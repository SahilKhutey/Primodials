import { useEffect, useState } from "react";

export type WallpaperViewport = {
  width: number;
  height: number;
  aspect: number;
  ultrawide: boolean;
  portrait: boolean;
};

function getViewport(): WallpaperViewport {
  if (typeof window === "undefined") {
    return {
      width: 1920,
      height: 1080,
      aspect: 16 / 9,
      ultrawide: false,
      portrait: false,
    };
  }
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
    if (typeof window === "undefined") return;
    const update = () => setViewport(getViewport());
    window.addEventListener("resize", update, { passive: true });
    return () => window.removeEventListener("resize", update);
  }, []);

  return viewport;
}
