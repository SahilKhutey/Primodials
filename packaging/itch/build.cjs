const fs = require('fs');
const path = require('path');
const { execSync } = require('child_process');

const rootDir = path.join(__dirname, '../..');
const distBrowserDir = path.join(rootDir, 'dist');
const distWallpaperDir = path.join(rootDir, 'dist-wallpaper');
const targetDir = path.join(rootDir, 'dist-itch');

if (!fs.existsSync(distBrowserDir)) {
  console.error('Error: dist/ not found. Please run "npm run build" first.');
  process.exit(1);
}

if (!fs.existsSync(distWallpaperDir)) {
  console.error('Error: dist-wallpaper/ not found. Please run "npm run build:wallpaper" first.');
  process.exit(1);
}

console.log('Packaging itch.io release artifacts in dist-itch/...');

fs.rmSync(targetDir, { recursive: true, force: true });
fs.mkdirSync(targetDir, { recursive: true });

const readmeText = [
  'PRIMORDIALS — Interactive Desktop Wallpaper Setup Guide',
  '======================================================',
  '',
  'Thank you for purchasing Primordials!',
  '',
  'How to run on your desktop:',
  '',
  'Option A: Wallpaper Engine (Steam)',
  '1. Open Wallpaper Engine.',
  '2. Click "Wallpaper Editor" or "Create Wallpaper".',
  '3. Drag and drop this folder (or the index.html file) into the editor.',
  '4. Save and apply as your desktop wallpaper!',
  '',
  'Option B: Lively Wallpaper (Free & Open Source for Windows)',
  '1. Download Lively Wallpaper (from Microsoft Store or https://rocksdanister.github.io/lively/).',
  '2. Click "Add Wallpaper" (+) in Lively.',
  '3. Select this folder or index.html.',
  '4. Customize and enjoy!',
  '',
  'Option C: Any Web Browser / Fullscreen Mode',
  '1. Double-click index.html to open in your favorite web browser (Chrome, Edge, Firefox).',
  '2. Press F11 for an immersive, distraction-free ambient simulation.',
  '',
  'Features & Themes:',
  '- Switch ambient themes (Default Dark, Bioluminescent Abyssal, Golden Savannah, Crystal Cave)',
  '- Ambient HUD with real-time ecosystem stats',
  '- Autonomous neural AI evolution with zero CPU bloat',
  '',
  'Enjoy your living desktop ecosystem!'
].join('\n');

fs.writeFileSync(path.join(targetDir, 'README-SETUP.txt'), readmeText);
fs.writeFileSync(path.join(distWallpaperDir, 'README-SETUP.txt'), readmeText);

const browserZipPath = path.join(targetDir, 'primordials-browser-demo.zip');
console.log('Creating browser-demo HTML5 zip...');
execSync(`powershell -Command "Compress-Archive -Path '${distBrowserDir}\\*' -DestinationPath '${browserZipPath}' -Force"`);

const wallpaperZipPath = path.join(targetDir, 'primordials-wallpaper-package.zip');
console.log('Creating downloadable wallpaper package zip...');
execSync(`powershell -Command "Compress-Archive -Path '${distWallpaperDir}\\*' -DestinationPath '${wallpaperZipPath}' -Force"`);

console.log('✓ Successfully generated itch.io release packages:');
console.log('  1. Browser Embed ZIP:       dist-itch/primordials-browser-demo.zip');
console.log('  2. Downloadable Package:    dist-itch/primordials-wallpaper-package.zip');
console.log('  3. Instructions:            dist-itch/README-SETUP.txt');
