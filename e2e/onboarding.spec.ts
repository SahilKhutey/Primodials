import { test, expect } from "@playwright/test";

test("fresh context shows onboarding", async ({ page }) => {
  await page.goto("/");
  const dialog = page.getByRole("dialog", { name: /primordials/i });
  await expect(dialog).toBeVisible();
  await expect(dialog).toContainText(/Watch/i);
  await expect(dialog).toContainText(/Inspect/i);
  await expect(dialog).toContainText(/Shape/i);
  await expect(dialog).toContainText(/Share/i);
  const nextBtn = page.getByRole("button", { name: /next|enter the ecosystem/i });
  for (let i = 0; i < 4; i++) {
    if (await nextBtn.isVisible().catch(() => false)) {
      await nextBtn.click();
      await page.waitForTimeout(100);
    }
  }
  await expect(dialog).toHaveCount(0);
});
