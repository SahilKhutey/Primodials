const fs = require('fs');
const path = require('path');

const wallpaperDistDir = path.join(__dirname, '../../dist-wallpaper');
const standardDistDir = path.join(__dirname, '../../dist');

// Prefer dedicated dist-wallpaper build if present, fallback to standard dist
const distDir = fs.existsSync(wallpaperDistDir) ? wallpaperDistDir : standardDistDir;
const targetDir = path.join(__dirname, '../../dist-wallpaper-engine');

if (!fs.existsSync(distDir)) {
  console.error('Error: neither dist-wallpaper/ nor dist/ found. Please run "npm run build:wallpaper" first.');
  process.exit(1);
}

console.log(`Packaging Wallpaper Engine artifact from ${path.basename(distDir)}...`);

fs.rmSync(targetDir, { recursive: true, force: true });
fs.mkdirSync(targetDir, { recursive: true });

// Copy dist contents to dist-wallpaper-engine
fs.cpSync(distDir, targetDir, { recursive: true });

// Copy project.json & preview.png to dist-wallpaper-engine
const projectJsonSrc = path.join(__dirname, 'project.json');
if (fs.existsSync(projectJsonSrc)) {
  fs.copyFileSync(projectJsonSrc, path.join(targetDir, 'project.json'));
}

const previewSrc = path.join(__dirname, 'preview.png');
if (fs.existsSync(previewSrc)) {
  fs.copyFileSync(previewSrc, path.join(targetDir, 'preview.png'));
}

console.log('✓ Successfully packaged for Wallpaper Engine in dist-wallpaper-engine/');
