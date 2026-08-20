# Fault Injection Matrix

| Fault | Injection | Expected outcome |
|---|---|---|
| Storage | `VITE_PP_FAULT=storage` | Persistence fails safely |
| Snapshot | `?fault=snapshot` | Snapshot path reports failure |
| World share | `?fault=world-share` | Share path rejects safely |
| Simulation | `?fault=simulation-step` | Runtime recovery/diagnostic path |
| Render | `?fault=render` | Error boundary/recovery |
| Wallpaper bridge | `VITE_PP_FAULT=wallpaper-bridge` | Bridge isolated |

## Safety

Fault injection is DEV-only.

Never ship a build where:
```text
activeFault() !== null
```

## Procedure

Inject → reproduce → capture → verify fallback → reload → verify recovery → disable → verify normal mode.
