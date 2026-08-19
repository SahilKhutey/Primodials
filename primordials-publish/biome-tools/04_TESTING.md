# 🧪 PRIMODIALS — Biome Authoring Suite Test Strategy

---

## 🔬 TEST SUITES

1. **Unit Testing (`ValidationEngine.test.ts`):** 100% coverage across all error (`E***`), warning (`W***`), and informational (`I***`) diagnostic codes.
2. **Schema Round-Trip Verification (`JsonExporter.test.ts`):** Verifies that imported JSON parses bit-for-bit into the active editor state and exports back to valid JSON without property loss.
3. **End-to-End Simulation Tests (Playwright):** Simulates user clicking "+ Add Species", adjusting sliders, and observing corresponding population curves on the live canvas.
4. **Visual Regression:** Playwright snapshot comparison ensuring UI components match the dark aesthetic design tokens.
