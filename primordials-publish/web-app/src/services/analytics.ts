/**
 * Privacy-friendly analytics via Plausible
 */
declare global {
  interface Window {
    plausible?: (
      event: string,
      options?: { props?: Record<string, string | number> }
    ) => void;
  }
}

export function initializeAnalytics(): void {
  const domain = import.meta.env.VITE_PLAUSIBLE_DOMAIN;
  const host = import.meta.env.VITE_PLAUSIBLE_HOST ?? 'https://plausible.io';
  if (!domain) return;

  const script = document.createElement('script');
  script.defer = true;
  script.dataset.domain = domain;
  script.src = `${host}/js/script.js`;
  document.head.appendChild(script);
}

export function trackEvent(
  event: string,
  props?: Record<string, string | number>
): void {
  if (typeof window !== 'undefined' && typeof window.plausible === 'function') {
    window.plausible(event, props ? { props } : undefined);
  }
}
