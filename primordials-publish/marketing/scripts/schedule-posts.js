#!/usr/bin/env node
/**
 * Primordials — Social Post Scheduler
 *
 * Reads launch schedule and exports JSON queue for Buffer/Hootsuite.
 */

import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

const launchDate = process.env.LAUNCH_DATE || '2026-08-15T13:00:00Z';

const posts = [
  {
    id: 'tw-001',
    platform: 'twitter',
    type: 'thread-root',
    scheduledFor: launchDate,
    content: `I spent 6 months building a desktop ecosystem that actually lives.\n\n🧬 Predators hunt, prey flee, balance emerges.\n\nIt's free, open source, and runs at 60 FPS.\n\nHere's the story 🧵👇`,
    media: ['demo-gif--algae-bloom--6s--twitter.gif'],
    hashtags: ['#indiedev', '#gamedev', '#opensource'],
  },
  {
    id: 'rd-001',
    platform: 'reddit',
    subreddit: 'wallpaperengine',
    scheduledFor: launchDate,
    title: 'I built an interactive predator-prey ecosystem wallpaper (60 FPS, low CPU, free & open source)',
    content: 'Hey r/wallpaperengine! Long-time lurker...\n\n[Full text in marketing/04_REDDIT_LAUNCH_KIT.md]',
  },
];

const scheduler = {
  generatedAt: new Date().toISOString(),
  launchDate,
  posts,
};

const outDir = path.join(__dirname, '../scheduled');
fs.mkdirSync(outDir, { recursive: true });
fs.writeFileSync(
  path.join(outDir, 'posts.json'),
  JSON.stringify(scheduler, null, 2)
);

console.log(`✅ Generated ${scheduler.posts.length} scheduled posts`);
console.log(`📅 Launch date: ${launchDate}`);
console.log(`📂 Output: ${outDir}/posts.json`);
