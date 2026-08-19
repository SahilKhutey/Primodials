/**
 * Primordials — Submit Biome to Community Modal Dialog
 *
 * License: MIT
 */

import React, { useState } from 'react';
import type { Biome } from '../../web-app/src/types/biome';
import { X, Upload, CheckCircle2 } from 'lucide-react';

interface SubmitDialogProps {
  biome: Biome;
  onClose: () => void;
}

export function SubmitDialog({ biome, onClose }: SubmitDialogProps) {
  const [author, setAuthor] = useState('');
  const [tags, setTags] = useState(biome.tags.join(', '));
  const [isSubmitting, setIsSubmitting] = useState(false);
  const [isSuccess, setIsSuccess] = useState(false);

  const handleSubmit = (e: React.FormEvent) => {
    e.preventDefault();
    setIsSubmitting(true);
    setTimeout(() => {
      setIsSubmitting(false);
      setIsSuccess(true);
    }, 1200);
  };

  return (
    <div className="fixed inset-0 z-50 flex items-center justify-center bg-bg-void/80 backdrop-blur-sm p-4">
      <div className="w-full max-w-md rounded-lg border border-bg-surface bg-bg-deep p-6 text-text-primary">
        <div className="flex items-center justify-between border-b border-bg-surface pb-3">
          <h2 className="font-display font-bold">Publish to Community Gallery</h2>
          <button onClick={onClose} className="rounded p-1 text-text-muted hover:text-text-primary">
            <X className="h-4 w-4" />
          </button>
        </div>

        {isSuccess ? (
          <div className="py-8 text-center">
            <CheckCircle2 className="mx-auto h-12 w-12 text-accent-teal" />
            <h3 className="mt-3 font-bold">Ecosystem Published!</h3>
            <p className="mt-1 text-xs text-text-muted">Your biome is now indexed in the public community gallery.</p>
            <button onClick={onClose} className="mt-5 rounded bg-accent-magenta px-4 py-2 text-xs font-bold text-bg-void">
              Done
            </button>
          </div>
        ) : (
          <form onSubmit={handleSubmit} className="mt-4 space-y-4 text-xs">
            <div>
              <label className="block text-text-muted">Author / GitHub Handle</label>
              <input
                type="text"
                required
                value={author}
                onChange={(e) => setAuthor(e.target.value)}
                placeholder="your_handle"
                className="mt-1 w-full rounded border border-bg-surface bg-bg-void px-3 py-1.5 focus:border-accent-magenta focus:outline-none"
              />
            </div>

            <div>
              <label className="block text-text-muted">Discovery Tags</label>
              <input
                type="text"
                value={tags}
                onChange={(e) => setTags(e.target.value)}
                placeholder="marine, hydrothermal, chaotic"
                className="mt-1 w-full rounded border border-bg-surface bg-bg-void px-3 py-1.5 focus:border-accent-magenta focus:outline-none"
              />
            </div>

            <div className="rounded border border-bg-surface/60 bg-bg-surface/30 p-3 text-[11px] text-text-muted">
              By submitting, you agree to release this biome schema under the MIT open source license for the Primordials ecosystem.
            </div>

            <div className="flex justify-end gap-2 pt-2">
              <button type="button" onClick={onClose} className="rounded px-3 py-1.5 text-text-muted hover:text-text-primary">
                Cancel
              </button>
              <button
                type="submit"
                disabled={isSubmitting}
                className="flex items-center gap-1.5 rounded bg-accent-magenta px-4 py-1.5 font-semibold text-bg-void hover:opacity-90 disabled:opacity-50"
              >
                <Upload className="h-3.5 w-3.5" />
                {isSubmitting ? 'Uploading...' : 'Publish Biome'}
              </button>
            </div>
          </form>
        )}
      </div>
    </div>
  );
}
