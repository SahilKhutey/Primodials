import { test, expect } from "./fixtures/app";

test("invalid world share does not blank the app", async ({ page, appReady }) => {
  void appReady;
  await page.goto("/?world=PP1.invalid");
  await expect(page.locator("canvas").first()).toBeVisible();
  await expect(page.locator("body")).not.toContainText(/unexpected runtime error/i);
});

test("capture presentation query is accepted", async ({ page, appReady }) => {
  void appReady;
  await page.goto("/?presentation=capture");
  await expect(page.locator("canvas").first()).toBeVisible();
});
