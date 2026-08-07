# 🚨 Critical Bug Response Protocol

## Severity Levels

### SEV-1: Critical Crash / Corrupting
- Crash on launch or main loop
- Unlist on Steamworks immediately
- Build hotfix within 24 hours

### SEV-2: Major Glitch
- Visual regression or UI lockout
- Hotfix within 72 hours

## Hotfix Pipeline
```bash
git checkout -b hotfix/v2.0.1
# Fix issue
python tools/build_depots.py --version 2.0.1
python tools/upload_to_steam.py --app-id 1234560 --depot-version 2.0.1
```
