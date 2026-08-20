# Release Artifact Validation

## Artifacts

Put exact release outputs into:
```text
release/artifacts/
```

Expected examples:
```text
PolygonalPrimordials-Setup.exe
PolygonalPrimordials-Windows-x64.zip
WallpaperEngine package
```

## Verify

```bash
node tools/validate-release.mjs
node tools/create-build-info.mjs
node tools/generate-release-manifest.mjs
```

Windows hash:
```powershell
powershell -ExecutionPolicy Bypass -File scripts/windows-release-artifact-check.ps1 -ArtifactPath "release\artifacts\PolygonalPrimordials-Setup.exe"
```

## Clean machine

On a machine without previous Primordials data:
- install;
- launch;
- complete onboarding;
- run simulation;
- save;
- close;
- reopen;
- recover;
- uninstall.

## Identity

Record:
- filename;
- version;
- SHA-256;
- commit;
- build date;
- platform;
- architecture.
