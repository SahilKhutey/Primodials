'use client';

import { Analytics as VercelAnalytics } from '@vercel/analytics/react';

export function Analytics() {
  if (process.env.NEXT_PUBLIC_VERCEL_ANALYTICS_ID) {
    return <VercelAnalytics />;
  }
  return null;
}
