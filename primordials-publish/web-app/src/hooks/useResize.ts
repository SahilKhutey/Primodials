import { useEffect, type RefObject } from 'react';

export function useResize(
  ref: RefObject<HTMLElement>,
  callback: () => void
): void {
  useEffect(() => {
    if (!ref.current) return;
    const observer = new ResizeObserver(() => callback());
    observer.observe(ref.current);
    return () => observer.disconnect();
  }, [ref, callback]);
}
