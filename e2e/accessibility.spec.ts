import { test, expect } from "./fixtures/app";

test("help dialog has an accessible name", async ({ page, appReady }) => {
  void appReady;
  const helpBtn = page.getByRole("button", { name: /open help/i });
  await helpBtn.click();
  await expect(page.getByRole("dialog", { name: /control your ecosystem/i })).toBeVisible();
});

test("reduced-motion context still starts", async ({ page, appReady }) => {
  void appReady;
  await expect(page.locator("canvas").first()).toBeVisible();
});
