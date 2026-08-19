import { describe, expect, it } from 'vitest';
import { FIRST_RUN_STEPS, PRODUCT_NAME, PRODUCT_TAGLINE } from '../src/lib/uxCopy';

describe('Phase 4 UX content', () => {
  it('has a defined product identity', () => {
    expect(PRODUCT_NAME).toBe('Polygonal Primordials');
    expect(PRODUCT_TAGLINE.length).toBeGreaterThan(10);
  });

  it('contains a complete first-run path', () => {
    expect(FIRST_RUN_STEPS.length).toBe(4);
    expect(FIRST_RUN_STEPS.map((step) => step.title)).toEqual([
      'Watch',
      'Inspect',
      'Shape',
      'Share',
    ]);
  });
});
