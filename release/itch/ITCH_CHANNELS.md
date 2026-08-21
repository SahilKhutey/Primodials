# itch.io Channels

Recommended channels:

- `windows-beta`
- `windows-stable`
- `linux-beta`
- `macos-beta`

Example:

```bash
butler login
node tools/publish-itch.mjs windows-beta release/artifacts/windows
```

Set:

```text
ITCH_TARGET=yourname/polygonal-primordials
```

Butler is itch.io's official CLI for pushing builds and supports named channels and differential updates. citeturn756202search0turn756202search4
