# 🔨 PRIMORDIALS — CMake Presets & Build Modernization

---

## ⚙️ CMAKE PRESETS ARCHITECTURE (`CMakePresets.json`)

```json
{
  "version": 6,
  "cmakeMinimumRequired": { "major": 3, "minor": 25 },
  "configurePresets": [
    {
      "name": "base",
      "hidden": true,
      "generator": "Ninja",
      "binaryDir": "${sourceDir}/build/${presetName}",
      "cacheVariables": {
        "CMAKE_CXX_STANDARD": "23",
        "CMAKE_EXPORT_COMPILE_COMMANDS": "ON"
      }
    },
    {
      "name": "release",
      "inherits": "base",
      "displayName": "Optimized Production",
      "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Release",
        "PRIMORDIALS_ENABLE_LTO": "ON"
      }
    }
  ]
}
```
