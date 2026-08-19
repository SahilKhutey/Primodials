import { type SelectHTMLAttributes } from 'react';
import { cn } from '@/utils/cn';

export interface SelectOption {
  label: string;
  value: string;
}

interface SelectProps extends SelectHTMLAttributes<HTMLSelectElement> {
  label?: string;
  options: SelectOption[];
}

export function Select({ label, options, className, ...props }: SelectProps) {
  return (
    <div className="space-y-1.5">
      {label && <label className="block font-mono text-xs text-text-muted">{label}</label>}
      <select
        className={cn(
          'w-full rounded-md border border-bg-surface bg-bg-surface/50 px-3 py-1.5 font-mono text-xs text-text-primary focus:border-accent-magenta focus:outline-none',
          className
        )}
        {...props}
      >
        {options.map((opt) => (
          <option key={opt.value} value={opt.value} className="bg-bg-deep text-text-primary">
            {opt.label}
          </option>
        ))}
      </select>
    </div>
  );
}
