# 🚀 Launch Day Runbook — Polygonal Primordials v2.0

## T-24 Hours: Final Prep
1. Run pre-launch checklist script:
   ```bash
   python tools/launch_checklist.py
   ```
2. Build Steam depots:
   ```bash
   python tools/build_depots.py --version 2.0.0
   ```
3. Upload to Steamworks:
   ```bash
   python tools/upload_to_steam.py --app-id 1234560 --depot-version 2.0.0
   ```

## T-0: Launch Day
10:00 AM — Click **"Release"** on Steamworks Partner site.
10:05 AM — Post launch announcements on Steam, Twitter, Discord.
10:15 AM — Start real-time monitoring:
```bash
python tools/monitor_launch.py
```
