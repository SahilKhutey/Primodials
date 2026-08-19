import type { Metadata, Viewport } from 'next';
import { Space_Grotesk, JetBrains_Mono } from 'next/font/google';
import Script from 'next/script';
import { siteConfig, createMetadata, softwareJsonLd, organizationJsonLd } from '@/lib/seo';
import { Analytics } from '@/components/analytics/Analytics';
import { Navbar } from '@/components/layout/Navbar';
import { Footer } from '@/components/layout/Footer';
import './globals.css';

const sans = Space_Grotesk({
  subsets: ['latin'],
  display: 'swap',
  variable: '--font-sans',
  weight: ['400', '500', '700', '900'],
});

const mono = JetBrains_Mono({
  subsets: ['latin'],
  display: 'swap',
  variable: '--font-mono',
  weight: ['400', '500', '700'],
});

export const metadata: Metadata = createMetadata();

export const viewport: Viewport = {
  themeColor: [
    { media: '(prefers-color-scheme: light)', color: '#F4F1DE' },
    { media: '(prefers-color-scheme: dark)', color: '#0F3D3E' },
  ],
  width: 'device-width',
  initialScale: 1,
  maximumScale: 5,
  colorScheme: 'dark',
};

export default function RootLayout({
  children,
}: Readonly<{
  children: React.ReactNode;
}>) {
  return (
    <html lang="en" className={`${sans.variable} ${mono.variable} dark`} suppressHydrationWarning>
      <head>
        {/* JSON-LD structured data */}
        <script
          type="application/ld+json"
          dangerouslySetInnerHTML={{
            __html: JSON.stringify([softwareJsonLd(), organizationJsonLd()]),
          }}
        />

        {/* Plausible Analytics */}
        {process.env.NEXT_PUBLIC_PLAUSIBLE_DOMAIN && (
          <Script
            defer
            data-domain={process.env.NEXT_PUBLIC_PLAUSIBLE_DOMAIN}
            src={`${process.env.NEXT_PUBLIC_PLAUSIBLE_HOST}/js/script.js`}
            strategy="afterInteractive"
          />
        )}

        <link rel="alternate" type="application/rss+xml" title={`${siteConfig.name} Blog`} href="/feed.xml" />
      </head>
      <body className="min-h-screen bg-bg-void font-sans text-text-primary antialiased">
        <a
          href="#main-content"
          className="sr-only focus:not-sr-only focus:absolute focus:left-4 focus:top-4 focus:z-50 focus:rounded-md focus:bg-accent-magenta focus:px-4 focus:py-2 focus:text-text-primary"
        >
          Skip to main content
        </a>
        <Navbar />
        <main id="main-content">{children}</main>
        <Footer />
        <Analytics />
      </body>
    </html>
  );
}
