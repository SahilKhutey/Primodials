import { test, expect } from "./fixtures/app";

test("application starts and renders canvas", async ({ page, appReady }) => {
  void appReady;
  await expect(page.locator("canvas").first()).toBeVisible();
  await expect(page.locator("body")).not.toContainText(/unexpected runtime error/i);
});

test("production does not show developer diagnostics by default", async ({ page, appReady }) => {
  void appReady;
  await expect(page.getByText(/release preflight/i)).toHaveCount(0);
  await expect(page.getByText(/runtime/i).filter({ hasText: /healthy|degraded/i })).toHaveCount(0);
});
