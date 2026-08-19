import { useState } from 'react';
import { ArrowRight, Check, Dna, Keyboard, Sparkles } from 'lucide-react';
import { FIRST_RUN_STEPS, PRODUCT_NAME, PRODUCT_TAGLINE } from '@/lib/uxCopy';

type Props = {
  open: boolean;
  onComplete: () => void;
};

export function FirstRunOverlay({ open, onComplete }: Props) {
  const [index, setIndex] = useState(0);

  if (!open) return null;

  const finalStep = index === FIRST_RUN_STEPS.length - 1;
  const step = FIRST_RUN_STEPS[index];

  return (
    <div
      className="fixed inset-0 z-[100] grid place-items-center bg-black/75 p-4 backdrop-blur-md"
      role="dialog"
      aria-modal="true"
      aria-labelledby="primordials-first-run-title"
    >
      <div className="w-full max-w-2xl overflow-hidden rounded-3xl border border-white/10 bg-neutral-950/95 shadow-2xl shadow-black/60">
        <div className="relative overflow-hidden border-b border-white/10 px-6 pb-6 pt-7 sm:px-8">
          <div className="absolute inset-0 bg-[radial-gradient(circle_at_80%_20%,rgba(34,211,238,0.14),transparent_35%),radial-gradient(circle_at_20%_80%,rgba(168,85,247,0.12),transparent_40%)]" />
          <div className="relative">
            <div className="mb-4 flex items-center gap-3">
              <div className="rounded-2xl bg-cyan-400/10 p-3 ring-1 ring-cyan-300/20">
                <Dna className="text-cyan-300" size={26} />
              </div>
              <div>
                <div className="text-[10px] font-semibold uppercase tracking-[0.2em] text-cyan-300/80">
                  Welcome
                </div>
                <h1 id="primordials-first-run-title" className="text-2xl font-bold text-white">
                  {PRODUCT_NAME}
                </h1>
              </div>
            </div>

            <p className="max-w-xl text-sm leading-6 text-neutral-400">
              {PRODUCT_TAGLINE} {step.body}
            </p>
          </div>
        </div>

        <div className="px-6 py-6 sm:px-8">
          <div className="mb-6 grid grid-cols-4 gap-2">
            {FIRST_RUN_STEPS.map((item, i) => (
              <button
                key={item.title}
                type="button"
                onClick={() => setIndex(i)}
                aria-label={`Go to step ${i + 1}: ${item.title}`}
                className={`rounded-xl p-2 text-left transition ${
                  i === index
                    ? 'bg-cyan-400/10 ring-1 ring-cyan-300/25'
                    : 'bg-white/[0.03] hover:bg-white/[0.06]'
                }`}
              >
                <div className="mb-2 flex items-center justify-between">
                  <span className="text-[10px] font-semibold uppercase tracking-wider text-neutral-500">
                    0{i + 1}
                  </span>
                  {i < index && <Check size={12} className="text-emerald-300" />}
                </div>
                <div className={`text-xs font-semibold ${i === index ? 'text-cyan-200' : 'text-neutral-300'}`}>
                  {item.title}
                </div>
              </button>
            ))}
          </div>

          <div className="rounded-2xl border border-white/10 bg-white/[0.03] p-5">
            <div className="mb-2 flex items-center gap-2 text-sm font-semibold text-white">
              {index === 0 && <Sparkles size={16} className="text-cyan-300" />}
              {index === 1 && <Dna size={16} className="text-violet-300" />}
              {index === 2 && <Sparkles size={16} className="text-amber-300" />}
              {index === 3 && <Check size={16} className="text-emerald-300" />}
              {step.title}
            </div>
            <p className="text-sm leading-6 text-neutral-400">{step.body}</p>
          </div>

          <div className="mt-5 flex items-center gap-2 rounded-xl bg-black/20 px-3 py-2.5 text-[11px] text-neutral-400">
            <Keyboard size={14} className="text-neutral-400" />
            <span><strong className="text-neutral-300">Space / P</strong> play · <strong className="text-neutral-300">R</strong> reset · <strong className="text-neutral-300">H / ?</strong> help · <strong className="text-neutral-300">Ctrl/Cmd+F</strong> fullscreen</span>
          </div>

          <div className="mt-6 flex items-center justify-between">
            <div className="flex gap-1.5">
              {FIRST_RUN_STEPS.map((item, i) => (
                <span
                  key={item.title}
                  className={`h-1.5 rounded-full transition-all ${
                    i === index ? 'w-7 bg-cyan-300' : 'w-1.5 bg-white/15'
                  }`}
                />
              ))}
            </div>

            <div className="flex gap-2">
              {index > 0 && (
                <button
                  type="button"
                  onClick={() => setIndex((value) => value - 1)}
                  className="rounded-xl bg-white/5 px-4 py-2.5 text-xs font-semibold text-neutral-300 hover:bg-white/10"
                >
                  Back
                </button>
              )}

              <button
                type="button"
                onClick={() => {
                  if (finalStep) onComplete();
                  else setIndex((value) => value + 1);
                }}
                className="flex items-center gap-2 rounded-xl bg-cyan-400/15 px-4 py-2.5 text-xs font-semibold text-cyan-200 ring-1 ring-cyan-300/20 hover:bg-cyan-400/20"
              >
                {finalStep ? 'Enter the ecosystem' : 'Next'}
                {finalStep ? <Check size={14} /> : <ArrowRight size={14} />}
              </button>
            </div>
          </div>
        </div>
      </div>
    </div>
  );
}
