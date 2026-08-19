import { ArrowRight, Github, Heart } from 'lucide-react';

export function CTA() {
  return (
    <section className="section">
      <div className="container-narrow">
        <div className="relative overflow-hidden rounded-2xl border border-bg-surface bg-gradient-hero p-12 text-center sm:p-16">
          <div aria-hidden="true" className="absolute inset-0 bg-dots opacity-30" />
          <div
            aria-hidden="true"
            className="absolute -top-20 left-1/2 -z-0 h-60 w-60 -translate-x-1/2 rounded-full bg-accent-magenta/30 blur-[100px]"
          />

          <div className="relative">
            <h2 className="h-display mb-6">
              Turn your desktop into a <span className="text-gradient">terrarium</span>.
            </h2>

            <p className="lead mx-auto max-w-xl">
              Free forever. Open source. MIT licensed. Download once, keep forever.
            </p>

            <div className="mt-10 flex flex-col items-center justify-center gap-3 sm:flex-row">
              <a
                href="https://gensoulslab.itch.io/polygonal-primordials"
                target="_blank"
                rel="noopener noreferrer"
                className="btn-primary"
              >
                Play in Browser (itch.io) <ArrowRight className="h-4 w-4" />
              </a>
              <a
                href="https://github.com/SahilKhutey/Primodials"
                target="_blank"
                rel="noopener noreferrer"
                className="btn-secondary"
              >
                <Github className="h-4 w-4" /> Star on GitHub
              </a>
            </div>

            <p className="mt-8 inline-flex items-center gap-2 font-mono text-xs text-text-muted">
              <Heart className="h-3 w-3 text-accent-magenta" /> Made with care by Sahil Khutey · Bilaspur, India
            </p>
          </div>
        </div>
      </div>
    </section>
  );
}
