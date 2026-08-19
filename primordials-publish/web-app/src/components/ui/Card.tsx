import { type ReactNode, type HTMLAttributes } from 'react';
import { cn } from '@/utils/cn';

export interface CardProps extends HTMLAttributes<HTMLDivElement> {
  children: ReactNode;
  variant?: 'default' | 'surface' | 'glass';
}

export function Card({
  className,
  variant = 'default',
  children,
  ...props
}: CardProps) {
  const variantClasses = {
    default: 'border border-bg-surface/40 bg-bg-deep/80 backdrop-blur-md',
    surface: 'border border-bg-surface bg-bg-surface/50',
    glass: 'border border-bg-surface/20 bg-bg-void/40 backdrop-blur-xl',
  }[variant];

  return (
    <div
      className={cn('rounded-lg p-4 transition-all', variantClasses, className)}
      {...props}
    >
      {children}
    </div>
  );
}
