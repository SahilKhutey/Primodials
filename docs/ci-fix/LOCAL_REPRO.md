
# Local Reproduction

## Web

```bash
npm ci
npm run lint
npm run typecheck
node Tools/run-test-matrix.mjs
```

## Native test target

Linux/macOS with Ninja:

```bash
cmake -G Ninja -S . -B build/test \
  -DCMAKE_BUILD_TYPE=Release \
  -DSHAPE_BUILD_TESTS=ON \
  -DSHAPE_BUILD_EXAMPLES=OFF \
  -DSHAPE_BUILD_BENCHMARKS=OFF

cmake --build build/test --target ShapeEngineTests --parallel 2

ctest --test-dir build/test --output-on-failure --verbose
```

Windows:

```powershell
cmake -G "Visual Studio 17 2022" -A x64 `
  -S . -B build/test `
  -DCMAKE_BUILD_TYPE=Release `
  -DSHAPE_BUILD_TESTS=ON `
  -DSHAPE_BUILD_EXAMPLES=OFF `
  -DSHAPE_BUILD_BENCHMARKS=OFF

cmake --build build/test --config Release --target ShapeEngineTests --parallel 2

ctest --test-dir build/test -C Release --output-on-failure --verbose
```

## Important

Capture the **first compiler error**, not the final summary.

Do not remove `-Werror` or `/WX` just to turn the build green.
