'use client';

import Link from 'next/link';
import { useEffect, useState } from 'react';
import { Menu, X, Github } from 'lucide-react';
import { Logo } from '@/components/ui/Logo';
import { GitHubStars } from '@/components/ui/GitHubStars';
import { cn } from '@/lib/cn';

const navLinks = [
  { href: '/#features', label: 'Features' },
  { href: '/#biomes', label: 'Biomes' },
  { href: '/#performance', label: 'Performance' },
  { href: 'https://github.com/SahilKhutey/Primodials/tree/main/docs', label: 'Docs' },
];

export function Navbar() {
  const [isOpen, setIsOpen] = useState(false);
  const [isScrolled, setIsScrolled] = useState(false);

  useEffect(() => {
    const handleScroll = () => setIsScrolled(window.scrollY > 8);
    window.addEventListener('scroll', handleScroll);
    return () => window.removeEventListener('scroll', handleScroll);
  }, []);

  return (
    <header
      className={cn(
        'sticky top-0 z-40 w-full transition-all duration-300',
        isScrolled
          ? 'border-b border-bg-surface/40 bg-bg-void/80 backdrop-blur-xl'
          : 'bg-transparent'
      )}
    >
      <nav
        className="container-wide flex h-16 items-center justify-between"
        aria-label="Main navigation"
      >
        <Link
          href="/"
          className="group flex items-center gap-3 transition-opacity hover:opacity-80"
        >
          <Logo size={32} />
          <span className="font-display text-lg font-bold tracking-tight">
            primordials<span className="text-accent-magenta">.</span>
          </span>
        </Link>

        <ul className="hidden items-center gap-1 md:flex">
          {navLinks.map((link) => (
            <li key={link.href}>
              <Link
                href={link.href}
                className="rounded-md px-3 py-2 text-sm font-medium text-text-muted transition-colors hover:bg-bg-surface hover:text-text-primary"
              >
                {link.label}
              </Link>
            </li>
          ))}
        </ul>

        <div className="hidden items-center gap-3 md:flex">
          <a
            href="https://gensoulslab.itch.io/polygonal-primordials"
            target="_blank"
            rel="noopener noreferrer"
            className="btn-secondary text-xs"
          >
            Play Online
          </a>
          <a
            href="https://github.com/SahilKhutey/Primodials"
            target="_blank"
            rel="noopener noreferrer"
            className="group flex items-center gap-2 rounded-md border border-bg-surface bg-bg-surface px-3 py-1.5 text-sm transition-all hover:border-accent-magenta"
          >
            <Github className="h-4 w-4" />
            <GitHubStars />
          </a>
        </div>

        <button
          onClick={() => setIsOpen(!isOpen)}
          className="rounded-md p-2 md:hidden"
          aria-label={isOpen ? 'Close menu' : 'Open menu'}
          aria-expanded={isOpen}
        >
          {isOpen ? <X className="h-6 w-6" /> : <Menu className="h-6 w-6" />}
        </button>
      </nav>

      {/* Mobile menu */}
      {isOpen && (
        <div className="border-t border-bg-surface bg-bg-void md:hidden">
          <div className="container-wide space-y-1 py-4">
            {navLinks.map((link) => (
              <Link
                key={link.href}
                href={link.href}
                onClick={() => setIsOpen(false)}
                className="block rounded-md px-3 py-2 text-base font-medium text-text-muted hover:bg-bg-surface hover:text-text-primary"
              >
                {link.label}
              </Link>
            ))}
            <div className="flex flex-col gap-2 pt-4">
              <a
                href="https://gensoulslab.itch.io/polygonal-primordials"
                target="_blank"
                rel="noopener noreferrer"
                className="btn-primary w-full"
              >
                Play Online (itch.io)
              </a>
              <a
                href="https://github.com/SahilKhutey/Primodials"
                target="_blank"
                rel="noopener noreferrer"
                className="btn-secondary w-full"
              >
                <Github className="h-4 w-4" /> GitHub
              </a>
            </div>
          </div>
        </div>
      )}
    </header>
  );
}
