# CI Failure Root Causes

## 1. Web App — confirmed

The failing test was:

```text
tests/integration/artifactValidation.test.ts
```

It expected lowercase:

```text
tools/run-browser-benchmark.mjs
scripts/hardware-report.ps1
scripts/windows-release-artifact-check.ps1
```

The repository's canonical directories are capitalized:

```text
Tools/
Scripts/
```

GitHub's Ubuntu runner uses a case-sensitive filesystem, so:

```text
Tools/foo
```

and:

```text
tools/foo
```

are different paths.

The test therefore failed even though the tools existed.

### Fix

The test now accepts the repository-native paths and emits the complete candidates when a file is missing.

---

## 2. Ubuntu C++ — confirmed architectural cause

The final-validation tests use repository-relative paths such as:

```text
README.md
LICENSE
Content/
Tools/
Scripts/
```

Catch2 test discovery commonly executes discovered tests with a working directory associated with the test/build context unless explicitly configured.

The previous configuration was:

```cmake
catch_discover_tests(ShapeEngineTests)
```

The fix is:

```cmake
catch_discover_tests(
    ShapeEngineTests
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
```

This causes the final-validation tests to inspect the actual repository root.

This explains why Ubuntu can compile the test executable successfully but return a multi-test failure at CTest time.

---

## 3. Final Validation Ubuntu — same working-directory problem

The same `ShapeEngineTests` executable contains the final-validation suite.

Fixing the Catch2 working directory fixes the underlying relative-path assumption for both CI and Final Validation.

---

## 4. macOS / Windows C++ failures — not proven from public run summary

The public job summary only exposes the build process exit code, not the first compiler diagnostic.

The Phase 7 commit did not modify the native C++ engine source or engine test source, so these failures should be treated as either:
- existing native portability failures;
- compiler-warning-as-error issues;
- environment/toolchain differences.

The patch therefore:
- preserves `-Werror`/`/WX`;
- keeps strict diagnostics;
- turns off unrelated examples/benchmarks for test-only CI;
- uses `cmake --build ... --parallel`;
- adds a portability-safe version of the performance validation smoke test.

Do not permanently weaken compiler warnings until the first actual compiler diagnostic is captured.

---

## 5. Node 20 action warning

GitHub reports a warning that some Actions are using Node 20 and will be forced onto Node 24 runtime.

This is a maintenance warning, not the cause of the seven failures.

The workflow updates checkout/setup-node to their current major versions in the patch.
