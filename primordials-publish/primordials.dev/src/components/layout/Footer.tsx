import Link from 'next/link';
import { Logo } from '@/components/ui/Logo';
import { Github, Twitter, Youtube, MessageCircle } from 'lucide-react';

const footerSections = [
  {
    title: 'Product',
    links: [
      { href: '/#features', label: 'Features' },
      { href: '/#biomes', label: 'Biomes' },
      { href: 'https://gensoulslab.itch.io/polygonal-primordials', label: 'itch.io Play' },
      { href: 'https://github.com/SahilKhutey/Primodials/releases', label: 'Desktop Downloads' },
    ],
  },
  {
    title: 'Developers',
    links: [
      { href: 'https://github.com/SahilKhutey/Primodials/tree/main/docs', label: 'Documentation' },
      { href: 'https://github.com/SahilKhutey/Primodials', label: 'GitHub Repository' },
      { href: 'https://github.com/SahilKhutey/Primodials/blob/main/docs/ARCHITECTURE.md', label: 'Architecture' },
      { href: 'https://github.com/SahilKhutey/Primodials/blob/main/docs/BIOME_SCHEMA.md', label: 'Biome Schema' },
    ],
  },
  {
    title: 'Community',
    links: [
      { href: 'https://discord.gg/primordials', label: 'Discord' },
      { href: 'https://twitter.com/PrimordialsGame', label: 'Twitter / X' },
      { href: 'https://github.com/SahilKhutey/Primodials/discussions', label: 'Discussions' },
    ],
  },
];

const socials = [
  { href: 'https://github.com/SahilKhutey/Primodials', label: 'GitHub', icon: Github },
  { href: 'https://twitter.com/PrimordialsGame', label: 'Twitter', icon: Twitter },
  { href: 'https://youtube.com/@primordials', label: 'YouTube', icon: Youtube },
  { href: 'https://discord.gg/primordials', label: 'Discord', icon: MessageCircle },
];

export function Footer() {
  return (
    <footer className="border-t border-bg-surface/40 bg-bg-void">
      <div className="container-wide py-16">
        <div className="grid grid-cols-2 gap-12 md:grid-cols-4 md:gap-8">
          {/* Brand */}
          <div className="col-span-2 md:col-span-1">
            <Link href="/" className="flex items-center gap-3">
              <Logo size={40} />
              <span className="font-display text-xl font-bold">
                primordials<span className="text-accent-magenta">.</span>
              </span>
            </Link>
            <p className="mt-4 max-w-xs text-sm leading-relaxed text-text-muted">
              A living ecosystem on your desktop. Real predator-prey dynamics, polygonal art, 60 FPS.
            </p>
            <div className="mt-6 flex gap-3">
              {socials.map((social) => {
                const Icon = social.icon;
                return (
                  <a
                    key={social.href}
                    href={social.href}
                    target="_blank"
                    rel="noopener noreferrer"
                    aria-label={social.label}
                    className="rounded-md border border-bg-surface p-2 transition-all hover:border-accent-magenta hover:text-accent-magenta"
                  >
                    <Icon className="h-4 w-4" />
                  </a>
                );
              })}
            </div>
          </div>

          {/* Sections */}
          {footerSections.map((section) => (
            <div key={section.title}>
              <h3 className="label-mono mb-4">{section.title}</h3>
              <ul className="space-y-3">
                {section.links.map((link) => (
                  <li key={link.label}>
                    <a
                      href={link.href}
                      className="text-sm text-text-muted transition-colors hover:text-accent-magenta"
                    >
                      {link.label}
                    </a>
                  </li>
                ))}
              </ul>
            </div>
          ))}
        </div>

        <div className="mt-16 flex flex-col items-start justify-between gap-4 border-t border-bg-surface/40 pt-8 sm:flex-row sm:items-center">
          <p className="text-sm text-text-muted">
            © {new Date().getFullYear()} Gensouls Lab · Built by Sahil Khutey
          </p>
          <p className="text-xs font-mono text-text-subtle">
            MIT licensed · 100% Offline-First · Zero telemetry
          </p>
        </div>
      </div>
    </footer>
  );
}
