const { contextBridge, ipcRenderer } = require('electron');

contextBridge.exposeInMainWorld('wallpaperAPI', {
  onAutoPause: (callback) => {
    ipcRenderer.on('auto-pause', (_event, pause) => callback(pause));
  },
});
