import { biomes } from '@/lib/biomes';

export function Biomes() {
  return (
    <section id="biomes" className="section bg-bg-void/50">
      <div className="container-wide">
        <div className="flex flex-col items-start justify-between gap-4 sm:flex-row sm:items-end">
          <div>
            <h2 className="label-mono mb-4">Six worlds</h2>
            <h3 className="h-1 max-w-xl">Choose your ecosystem.</h3>
            <p className="lead mt-4 max-w-xl">
              Each biome is a real-world reference, tuned to feel alive. Or design your own — the JSON schema is open.
            </p>
          </div>
          <a
            href="https://github.com/SahilKhutey/Primodilas/blob/main/docs/BIOME_SCHEMA.md"
            target="_blank"
            rel="noopener noreferrer"
            className="btn-secondary text-xs"
          >
            Biome Schema Docs →
          </a>
        </div>

        <ul className="mt-12 grid gap-6 sm:grid-cols-2 lg:grid-cols-3">
          {biomes.map((biome) => (
            <li
              key={biome.id}
              className="group relative overflow-hidden rounded-xl border border-bg-surface/60 bg-bg-surface/30 backdrop-blur-sm transition-all hover:border-accent-magenta"
            >
              {/* Visual Thumbnail */}
              <div
                className="relative aspect-video w-full flex items-center justify-center"
                style={{
                  background: `linear-gradient(135deg, ${biome.color}22 0%, #0F3D3E 100%)`,
                }}
              >
                <div
                  className="font-mono text-sm font-bold uppercase tracking-widest"
                  style={{ color: biome.color }}
                >
                  {biome.emoji} {biome.name}
                </div>
              </div>

              {/* Content */}
              <div className="space-y-2 p-6">
                <div className="flex items-center justify-between">
                  <h4 className="text-lg font-semibold">{biome.name}</h4>
                  <span
                    className="h-3 w-3 rounded-full shadow-[0_0_8px]"
                    style={{
                      backgroundColor: biome.color,
                      boxShadow: `0 0 10px ${biome.color}`,
                    }}
                  />
                </div>
                <p className="font-mono text-xs uppercase tracking-wide text-text-muted">
                  {biome.tagline}
                </p>
                <p className="mt-2 text-sm leading-relaxed text-text-muted">
                  {biome.description}
                </p>
                <dl className="mt-4 flex gap-6 border-t border-bg-surface/40 pt-4 font-mono text-xs text-text-muted">
                  <div>
                    <dt>Species</dt>
                    <dd className="text-text-primary font-bold">{biome.species}</dd>
                  </div>
                  <div>
                    <dt>Typical Density</dt>
                    <dd className="text-text-primary font-bold">{biome.population}</dd>
                  </div>
                </dl>
              </div>
            </li>
          ))}
        </ul>
      </div>
    </section>
  );
}
