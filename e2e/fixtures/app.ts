/* eslint-disable react-hooks/rules-of-hooks */
import { expect, test as base } from "@playwright/test";

export const test = base.extend<{ appReady: void }>({
  appReady: async ({ page }, use) => {
    await page.addInitScript(() => {
      window.localStorage.setItem(
        "polygonal-primordials.first-run.v1",
        JSON.stringify({ version: 1, completed: true, completedAt: new Date().toISOString() })
      );
    });
    await page.goto("/");
    const enter = page.getByRole("button", { name: /enter the ecosystem/i });
    if (await enter.isVisible().catch(() => false)) await enter.click();
    await expect(page.locator("canvas").first()).toBeVisible();
    await use();
  },
});

export { expect };
