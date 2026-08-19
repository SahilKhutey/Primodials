/** @type {import('next').NextConfig} */
const nextConfig = {
  reactStrictMode: true,
  poweredByHeader: false,
  compress: true,

  // Image optimization
  images: {
    formats: ['image/avif', 'image/webp'],
    remotePatterns: [
      {
        protocol: 'https',
        hostname: 'raw.githubusercontent.com',
        pathname: '/SahilKhutey/Primodials/**',
      },
      {
        protocol: 'https',
        hostname: 'img.itch.zone',
      },
      {
        protocol: 'https',
        hostname: 'shared.cloudflare.steamstatic.com',
      },
    ],
    deviceSizes: [640, 750, 828, 1080, 1200, 1920, 2048, 3840],
    imageSizes: [16, 32, 48, 64, 96, 128, 256, 384],
  },

  // Security headers
  async headers() {
    return [
      {
        source: '/(.*)',
        headers: [
          { key: 'X-Frame-Options', value: 'SAMEORIGIN' },
          { key: 'X-Content-Type-Options', value: 'nosniff' },
          { key: 'Referrer-Policy', value: 'strict-origin-when-cross-origin' },
          { key: 'Permissions-Policy', value: 'camera=(), microphone=(), geolocation=()' },
        ],
      },
    ];
  },

  // Redirects
  async redirects() {
    return [
      { source: '/github', destination: 'https://github.com/SahilKhutey/Primodials', permanent: false },
      { source: '/discord', destination: 'https://discord.gg/primordials', permanent: false },
      { source: '/twitter', destination: 'https://twitter.com/PrimordialsGame', permanent: false },
      { source: '/itch', destination: 'https://gensoulslab.itch.io/polygonal-primordials', permanent: false },
      { source: '/steam', destination: 'https://store.steampowered.com/app/XXXXXXX', permanent: false },
      { source: '/wallpaper', destination: 'https://steamcommunity.com/sharedfiles/filedetails/?id=XXXXX', permanent: false },
    ];
  },

  // Performance: experimental features
  experimental: {
    optimizePackageImports: ['lucide-react', 'framer-motion'],
  },

  trailingSlash: false,
};

module.exports = nextConfig;
