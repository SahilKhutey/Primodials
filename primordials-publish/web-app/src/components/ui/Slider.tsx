import { cn } from '@/utils/cn';

interface SliderProps {
  label: string;
  value: number;
  min: number;
  max: number;
  step?: number;
  onChange: (value: number) => void;
  className?: string;
}

export function Slider({
  label,
  value,
  min,
  max,
  step = 1,
  onChange,
  className,
}: SliderProps) {
  const percentage = ((value - min) / (max - min)) * 100;

  return (
    <div className={cn('space-y-1.5', className)}>
      <div className="flex items-center justify-between font-mono text-xs">
        <span className="text-text-muted">{label}</span>
        <span className="text-text-primary">{value.toFixed(2)}</span>
      </div>
      <div className="relative h-1.5 rounded-full bg-bg-surface">
        <div
          className="absolute h-full rounded-full bg-accent-magenta transition-all"
          style={{ width: `${percentage}%` }}
        />
        <input
          type="range"
          min={min}
          max={max}
          step={step}
          value={value}
          onChange={(e) => onChange(parseFloat(e.target.value))}
          className="absolute inset-0 h-full w-full cursor-pointer appearance-none bg-transparent"
        />
      </div>
    </div>
  );
}
