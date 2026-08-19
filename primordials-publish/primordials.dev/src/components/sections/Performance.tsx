import { TrendingUp, Cpu, HardDrive, ShieldCheck } from 'lucide-react';

const benchmarks = [
  { label: 'Entities @ 60 FPS', value: '250K+', icon: TrendingUp, accent: 'text-biome-algae' },
  { label: 'Memory / 1K entities', value: '3.2 KB', icon: HardDrive, accent: 'text-accent-magenta' },
  { label: 'CPU Overhead (Idle)', value: '< 1.5%', icon: Cpu, accent: 'text-accent-violet' },
  { label: 'Test Assertion Suite', value: '1,311 Passing', icon: ShieldCheck, accent: 'text-biome-tundra' },
];

const codeBlock = `// Continuous Lotka-Volterra Euler Integrator
struct EcosystemDynamics {
    float alpha = 0.045f; // Prey reproduction
    float beta  = 0.025f; // Predation rate
    float delta = 0.015f; // Predator reproduction
    float gamma = 0.020f; // Predator mortality

    void step(float& prey, float& predator, float dt) {
        float dx = (alpha * prey - beta * prey * predator) * dt;
        float dy = (delta * prey * predator - gamma * predator) * dt;
        prey     = std::max(0.0f, prey + dx);
        predator = std::max(0.0f, predator + dy);
    }
};`;

export function Performance() {
  return (
    <section id="performance" className="section relative overflow-hidden">
      <div aria-hidden="true" className="absolute inset-0 -z-10 bg-gradient-to-b from-bg-void via-bg-deep/20 to-bg-void" />
      <div className="container-wide">
        <div className="grid gap-12 lg:grid-cols-2 lg:gap-16">
          {/* Numbers */}
          <div>
            <h2 className="label-mono mb-4">Engineered for Speed</h2>
            <h3 className="h-1">
              60 FPS locked, even at 250,000 organisms.
            </h3>
            <p className="lead mt-6">
              Built on a custom C++23 Entity-Component-System with archetype storage, spatial hashing, and bit-exact determinism. 221 unit tests. 1,311 assertions. Zero compromises.
            </p>
            <dl className="mt-10 grid grid-cols-2 gap-6">
              {benchmarks.map((bench) => {
                const Icon = bench.icon;
                return (
                  <div
                    key={bench.label}
                    className="rounded-lg border border-bg-surface bg-bg-surface/30 p-4"
                  >
                    <Icon className={`h-5 w-5 ${bench.accent}`} />
                    <dt className="mt-3 font-mono text-xs uppercase tracking-wider text-text-muted">
                      {bench.label}
                    </dt>
                    <dd className="font-display mt-1 text-2xl font-black">
                      {bench.value}
                    </dd>
                  </div>
                );
              })}
            </dl>
          </div>

          {/* Code */}
          <div className="rounded-xl border border-bg-surface bg-bg-void/80 p-1 shadow-glow-magenta">
            <div className="flex items-center gap-2 border-b border-bg-surface px-4 py-3">
              <div className="flex gap-1.5">
                <div className="h-3 w-3 rounded-full bg-biome-deepsea/60" />
                <div className="h-3 w-3 rounded-full bg-biome-soup/60" />
                <div className="h-3 w-3 rounded-full bg-biome-tundra/60" />
              </div>
              <span className="ml-2 font-mono text-xs text-text-muted">
                Ecology.hpp — C++23 Core
              </span>
            </div>
            <pre className="overflow-x-auto p-6 font-mono text-xs leading-relaxed text-text-primary">
              <code>{codeBlock}</code>
            </pre>
          </div>
        </div>
      </div>
    </section>
  );
}
