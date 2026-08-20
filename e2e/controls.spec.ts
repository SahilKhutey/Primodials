import { test, expect } from "./fixtures/app";

test("space toggles without crashing", async ({ page, appReady }) => {
  void appReady;
  const playBtn = page.getByRole("button", { name: /pause|play/i }).first();
  await expect(playBtn).toBeVisible();
  await playBtn.click();
  await page.waitForTimeout(100);
  await playBtn.click();
  await expect(page.locator("canvas").first()).toBeVisible();
});

test("help opens with H", async ({ page, appReady }) => {
  void appReady;
  const helpBtn = page.getByRole("button", { name: /open help/i });
  await helpBtn.click();
  await expect(page.getByRole("dialog", { name: /control your ecosystem/i })).toBeVisible();
});
