const { app, BrowserWindow, powerMonitor, ipcMain } = require('electron');
const path = require('path');

let mainWindow = null;

function createWindow() {
  mainWindow = new BrowserWindow({
    width: 1280,
    height: 720,
    frame: false,
    transparent: false,
    fullscreen: true,
    skipTaskbar: false,
    webPreferences: {
      preload: path.join(__dirname, 'preload.js'),
      nodeIntegration: false,
      contextIsolation: true,
    },
  });

  const isDev = process.env.NODE_ENV === 'development' || !app.isPackaged;

  if (isDev) {
    mainWindow.loadURL('http://localhost:5173/?wallpaper=1');
  } else {
    mainWindow.loadFile(path.join(__dirname, '../dist/index.html'), {
      query: { wallpaper: '1' },
    });
  }

  // Handle power monitor events (auto-pause on gaming / sleep)
  powerMonitor.on('suspend', () => {
    mainWindow?.webContents.send('auto-pause', true);
  });

  powerMonitor.on('resume', () => {
    mainWindow?.webContents.send('auto-pause', false);
  });
}

app.whenReady().then(() => {
  createWindow();

  app.on('activate', () => {
    if (BrowserWindow.getAllWindows().length === 0) createWindow();
  });
});

app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') app.quit();
});
