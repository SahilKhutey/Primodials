import type { MetadataRoute } from 'next';
import { siteConfig } from '@/lib/seo';

export default function sitemap(): MetadataRoute.Sitemap {
  const now = new Date();
  const routes = ['', '/features', '/biomes', '/gallery', '/download', '/docs', '/press', '/legal/privacy', '/legal/terms'];

  return routes.map((route) => ({
    url: `${siteConfig.url}${route}`,
    lastModified: now,
    changeFrequency: route === '' ? 'weekly' : 'monthly',
    priority: route === '' ? 1 : route === '/download' ? 0.9 : 0.7,
  }));
}
