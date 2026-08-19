import type { Metadata } from 'next';

export const siteConfig = {
  name: 'Primordials',
  tagline: 'A living ecosystem on your desktop.',
  description: 'Real Lotka-Volterra predator-prey dynamics running at 60 FPS on your desktop. Free, open-source, MIT licensed.',
  url: 'https://primordials.dev',
  ogImage: '/og/og-default.png',
  twitterHandle: '@PrimordialsGame',
  github: 'https://github.com/SahilKhutey/Primodials',
  discord: 'https://discord.gg/primordials',
  keywords: [
    'ecosystem',
    'simulation',
    'wallpaper',
    'screensaver',
    'live wallpaper',
    'desktop',
    'predator prey',
    'lotka-volterra',
    'open source',
    'cpp',
    'typescript',
    'ambient',
    'cozy',
    'art',
    'generative',
  ] as const,
  authors: [
    {
      name: 'Sahil Khutey',
      url: 'https://github.com/SahilKhutey',
    },
  ],
  creator: 'Sahil Khutey',
  publisher: 'Gensouls Lab',
};

export function createMetadata(overrides?: Partial<Metadata>): Metadata {
  return {
    metadataBase: new URL(siteConfig.url),
    title: {
      default: `${siteConfig.name} — ${siteConfig.tagline}`,
      template: `%s — ${siteConfig.name}`,
    },
    description: siteConfig.description,
    keywords: Array.from(siteConfig.keywords),
    authors: siteConfig.authors,
    creator: siteConfig.creator,
    publisher: siteConfig.publisher,
    robots: {
      index: true,
      follow: true,
      googleBot: {
        index: true,
        follow: true,
        'max-video-preview': -1,
        'max-image-preview': 'large',
        'max-snippet': -1,
      },
    },
    openGraph: {
      type: 'website',
      locale: 'en_US',
      url: siteConfig.url,
      title: `${siteConfig.name} — ${siteConfig.tagline}`,
      description: siteConfig.description,
      siteName: siteConfig.name,
      images: [
        {
          url: siteConfig.ogImage,
          width: 1200,
          height: 630,
          alt: siteConfig.tagline,
        },
      ],
    },
    twitter: {
      card: 'summary_large_image',
      title: `${siteConfig.name} — ${siteConfig.tagline}`,
      description: siteConfig.description,
      creator: siteConfig.twitterHandle,
      images: [siteConfig.ogImage],
    },
    icons: {
      icon: [
        { url: '/favicon/favicon.svg', type: 'image/svg+xml' },
        { url: '/favicon/favicon-32x32.png', sizes: '32x32', type: 'image/png' },
        { url: '/favicon/favicon-16x16.png', sizes: '16x16', type: 'image/png' },
      ],
      apple: [{ url: '/favicon/apple-touch-icon.png', sizes: '180x180' }],
      other: [{ rel: 'mask-icon', url: '/favicon/safari-pinned-tab.svg', color: '#0F3D3E' }],
    },
    manifest: '/favicon/site.webmanifest',
    alternates: {
      canonical: siteConfig.url,
    },
    ...overrides,
  };
}

export function softwareJsonLd() {
  return {
    '@context': 'https://schema.org',
    '@type': 'SoftwareApplication',
    name: siteConfig.name,
    description: siteConfig.description,
    url: siteConfig.url,
    image: `${siteConfig.url}${siteConfig.ogImage}`,
    author: {
      '@type': 'Person',
      name: 'Sahil Khutey',
      url: 'https://github.com/SahilKhutey',
    },
    publisher: {
      '@type': 'Organization',
      name: 'Gensouls Lab',
    },
    applicationCategory: 'MultimediaApplication',
    applicationSubCategory: 'DesktopEnhancementApplication',
    operatingSystem: 'Windows 10+, macOS 12+, Linux Ubuntu 22.04+',
    offers: {
      '@type': 'Offer',
      price: '0',
      priceCurrency: 'USD',
      availability: 'https://schema.org/InStock',
    },
    downloadUrl: siteConfig.github,
    softwareVersion: '2.0.0',
    softwareRequirements: 'OpenGL 3.3+, 2GB RAM',
    featureList: [
      'Real Lotka-Volterra predator-prey dynamics',
      '250,000 entities at 60 FPS',
      '6 biomes included',
      'JSON-configurable custom biomes',
      'Open source (MIT)',
    ],
    license: 'https://opensource.org/licenses/MIT',
  };
}

export function organizationJsonLd() {
  return {
    '@context': 'https://schema.org',
    '@type': 'Organization',
    name: 'Gensouls Lab',
    url: 'https://primordials.dev',
    logo: `${siteConfig.url}/logo/logo-primary.svg`,
    sameAs: [
      siteConfig.github,
      siteConfig.discord,
      'https://twitter.com/PrimordialsGame',
    ],
    founder: {
      '@type': 'Person',
      name: 'Sahil Khutey',
    },
  };
}
