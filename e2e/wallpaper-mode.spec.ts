import { test, expect } from "@playwright/test";

test("wallpaper mode renders", async ({ page }) => {
  await page.goto("/?wallpaper=1");
  await expect(page.locator("canvas").first()).toBeVisible();
});

test("ultrawide renders", async ({ browser }) => {
  const context = await browser.newContext({ viewport: { width: 3440, height: 1440 } });
  const page = await context.newPage();
  await page.goto("/?wallpaper=1");
  await expect(page.locator("canvas").first()).toBeVisible();
  await context.close();
});
