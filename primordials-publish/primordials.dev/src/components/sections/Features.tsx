import { Brain, Leaf, Zap, Palette, Monitor, Save, Wrench, Globe } from 'lucide-react';

const features = [
  {
    icon: Brain,
    title: 'Real Ecology',
    description: 'Lotka-Volterra predator-prey equations — authentic mathematical biology running continuous population curves.',
    accent: 'text-biome-algae',
    bg: 'bg-biome-algae/10',
  },
  {
    icon: Leaf,
    title: '6 Dynamic Biomes',
    description: 'Algae Bloom, Deep Sea, Primordial Soup, Tundra, Coral Reef, and Volcanic Vent. Each a unique biological balance.',
    accent: 'text-biome-tundra',
    bg: 'bg-biome-tundra/10',
  },
  {
    icon: Zap,
    title: '250K @ 60 FPS',
    description: 'Custom data-oriented C++23 ECS engine. Locked 60 FPS with <1.5% CPU overhead on modern systems.',
    accent: 'text-accent-amber',
    bg: 'bg-accent-amber/10',
  },
  {
    icon: Palette,
    title: 'Polygonal Art',
    description: 'Distinctive bioluminescent vector direction with customized glow parameters and dark void palettes.',
    accent: 'text-accent-magenta',
    bg: 'bg-accent-magenta/10',
  },
  {
    icon: Monitor,
    title: '3 Display Modes',
    description: 'Runs as a zero-friction live desktop wallpaper, screensaver, or interactive windowed laboratory.',
    accent: 'text-accent-violet',
    bg: 'bg-accent-violet/10',
  },
  {
    icon: Save,
    title: '100% Offline-First',
    description: 'Runs entirely on your local GPU/CPU. Zero required network calls; optional cloud saves via Supabase.',
    accent: 'text-biome-coral',
    bg: 'bg-biome-coral/10',
  },
  {
    icon: Wrench,
    title: 'Custom JSON Biomes',
    description: 'Extensible JSON schema for creators. Tweak carrying capacities, mutation rates, and species behaviors.',
    accent: 'text-biome-volcanic',
    bg: 'bg-biome-volcanic/10',
  },
  {
    icon: Globe,
    title: 'Open Source (MIT)',
    description: 'Permissive MIT license. Zero telemetry, zero analytics tracking, no ads. Read the code, fork it, and learn.',
    accent: 'text-biome-deepsea',
    bg: 'bg-biome-deepsea/10',
  },
];

export function Features() {
  return (
    <section id="features" className="section">
      <div className="container-wide">
        <div className="mx-auto max-w-2xl text-center">
          <h2 className="label-mono mb-4">Why Primordials</h2>
          <h3 className="h-1">Built for the patient observer.</h3>
          <p className="lead mt-6">
            Every screensaver you've ever used has felt dead. Primordials is the opposite — patient, alive, and quietly hypnotic.
          </p>
        </div>

        <ul className="mt-16 grid gap-6 sm:grid-cols-2 lg:grid-cols-4">
          {features.map((feature) => {
            const Icon = feature.icon;
            return (
              <li key={feature.title} className="card group">
                <div className={`mb-4 inline-flex rounded-md p-3 ${feature.bg} ${feature.accent}`}>
                  <Icon className="h-6 w-6" />
                </div>
                <h4 className="text-lg font-semibold">{feature.title}</h4>
                <p className="mt-2 text-sm leading-relaxed text-text-muted">
                  {feature.description}
                </p>
              </li>
            );
          })}
        </ul>
      </div>
    </section>
  );
}
