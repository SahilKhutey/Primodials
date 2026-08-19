import type { ReactNode } from 'react';

type Props = {
  title: string;
  description?: string;
  children: ReactNode;
  tone?: 'neutral' | 'cyan' | 'violet' | 'amber' | 'emerald';
};

const toneClass = {
  neutral: 'border-white/8 bg-white/[0.025]',
  cyan: 'border-cyan-300/10 bg-cyan-300/[0.03]',
  violet: 'border-violet-300/10 bg-violet-300/[0.03]',
  amber: 'border-amber-300/10 bg-amber-300/[0.03]',
  emerald: 'border-emerald-300/10 bg-emerald-300/[0.03]',
};

export function SettingsSection({
  title,
  description,
  children,
  tone = 'neutral',
}: Props) {
  return (
    <section className={`rounded-2xl border p-4 ${toneClass[tone]}`}>
      <div className="mb-3">
        <h3 className="text-xs font-semibold text-neutral-200">{title}</h3>
        {description && (
          <p className="mt-1 text-[11px] leading-5 text-neutral-500">{description}</p>
        )}
      </div>
      <div className="space-y-3">{children}</div>
    </section>
  );
}

export function SettingsRow({
  label,
  description,
  children,
}: {
  label: string;
  description?: string;
  children: ReactNode;
}) {
  return (
    <div className="flex items-center justify-between gap-4 rounded-xl bg-black/10 px-3 py-2.5">
      <div className="min-w-0">
        <div className="text-xs font-medium text-neutral-300">{label}</div>
        {description && (
          <div className="mt-0.5 text-[10px] leading-4 text-neutral-600">{description}</div>
        )}
      </div>
      <div className="shrink-0">{children}</div>
    </div>
  );
}
