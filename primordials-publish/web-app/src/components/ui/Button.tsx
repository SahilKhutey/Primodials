import React, { type ButtonHTMLAttributes } from 'react';
import { cn } from '@/utils/cn';

export interface ButtonProps extends ButtonHTMLAttributes<HTMLButtonElement> {
  variant?: 'primary' | 'secondary' | 'ghost';
  size?: 'sm' | 'md' | 'lg';
}

export function Button({
  className,
  variant = 'primary',
  size = 'md',
  children,
  ...props
}: ButtonProps) {
  const variantClasses = {
    primary: 'bg-accent-magenta text-bg-void hover:scale-[1.02] active:scale-[0.98]',
    secondary: 'border border-bg-surface bg-bg-surface/40 text-text-primary hover:border-accent-magenta',
    ghost: 'text-text-muted hover:bg-bg-surface hover:text-text-primary',
  }[variant];

  const sizeClasses = {
    sm: 'px-2.5 py-1.5 text-xs',
    md: 'px-4 py-2 text-sm',
    lg: 'px-6 py-3 text-base font-semibold',
  }[size];

  return (
    <button
      className={cn(
        'inline-flex items-center justify-center gap-2 rounded-md font-medium transition-all focus:outline-none focus:ring-2 focus:ring-accent-magenta disabled:pointer-events-none disabled:opacity-50',
        variantClasses,
        sizeClasses,
        className
      )}
      {...props}
    >
      {children}
    </button>
  );
}
