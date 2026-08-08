import { useState, useEffect } from 'react';

export type QualityPreset = 'low' | 'medium' | 'high';

export interface WallpaperSettings {
  maxPopulation: number;
  mutationRate: number;
  quality: QualityPreset;
  autoPause: boolean;
}

const STORAGE_KEY = 'polygonal_primordials_wallpaper_settings';

const DEFAULT_WALLPAPER_SETTINGS: WallpaperSettings = {
  maxPopulation: 250,
  mutationRate: 0.15,
  quality: 'medium',
  autoPause: true,
};

export function useWallpaperSettings() {
  const [wallpaperSettings, setWallpaperSettings] = useState<WallpaperSettings>(() => {
    try {
      const saved = localStorage.getItem(STORAGE_KEY);
      if (saved) {
        return { ...DEFAULT_WALLPAPER_SETTINGS, ...JSON.parse(saved) };
      }
    } catch (e) {
      console.warn('Failed to load wallpaper settings from localStorage:', e);
    }
    return DEFAULT_WALLPAPER_SETTINGS;
  });

  useEffect(() => {
    try {
      localStorage.setItem(STORAGE_KEY, JSON.stringify(wallpaperSettings));
    } catch (e) {
      console.warn('Failed to save wallpaper settings to localStorage:', e);
    }
  }, [wallpaperSettings]);

  const updateSetting = <K extends keyof WallpaperSettings>(
    key: K,
    value: WallpaperSettings[K]
  ) => {
    setWallpaperSettings((prev) => ({
      ...prev,
      [key]: value,
    }));
  };

  return {
    wallpaperSettings,
    updateSetting,
  };
}
