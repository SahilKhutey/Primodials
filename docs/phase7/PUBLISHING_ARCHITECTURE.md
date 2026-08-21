# Phase 7 — Publishing Architecture

## Goal

Separate the runtime application from commercial distribution.

```text
SOURCE
  ↓
QA BUILD
  ↓
RELEASE BUILD
  ↓
ARTIFACTS
  ├── Wallpaper Engine
  ├── itch.io
  └── Steam
```

## Release channels

```text
development
qa
beta
stable
```

Runtime code should never contain publishing credentials.

## Local-first telemetry

Phase 7 records only local first-launch/session events. No remote analytics endpoint is introduced.

## itch.io

Use Butler:

```bash
butler login
node tools/publish-itch.mjs windows-beta release/artifacts/windows
```

Butler supports automated uploads and channel-based delivery. citeturn756202search3turn756202search4

## Steam

Steam publishing remains deliberately human-controlled because:
- credentials must remain outside source;
- store configuration is external;
- review is external;
- launch timing is externally governed.

## Definition of done

- [ ] Release channel system
- [ ] Local first-launch/session events
- [ ] Publish preflight
- [ ] itch.io upload path
- [ ] Steam store manifest/template
- [ ] Steam release plan
- [ ] Wallpaper Engine checklist
- [ ] No credentials in repository
