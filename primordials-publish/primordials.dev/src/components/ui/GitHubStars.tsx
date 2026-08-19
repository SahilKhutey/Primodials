'use client';

import { useEffect, useState } from 'react';
import { Star } from 'lucide-react';

interface GitHubStarsProps {
  repo?: string;
  className?: string;
}

export function GitHubStars({
  repo = 'SahilKhutey/Primodials',
  className,
}: GitHubStarsProps) {
  const [stars, setStars] = useState<number | null>(null);

  useEffect(() => {
    const fetchStars = async () => {
      try {
        const res = await fetch(`https://api.github.com/repos/${repo}`);
        if (res.ok) {
          const data = await res.json();
          setStars(data.stargazers_count);
        }
      } catch {
        // Silently fallback if offline or rate-limited
      }
    };
    fetchStars();
  }, [repo]);

  if (stars === null) return <span>Star</span>;

  return (
    <span className={className}>
      <Star className="mr-1 inline-block h-3.5 w-3.5 fill-current" />
      {stars.toLocaleString()}
    </span>
  );
}
