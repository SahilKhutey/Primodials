import { test, expect } from "./fixtures/app";

test("world sharing produces PP1 payload", async ({ page, appReady }) => {
  void appReady;
  const share = page.getByRole("button", { name: /share/i }).first();
  if (!(await share.isVisible().catch(() => false))) test.skip(true, "Share control not exposed.");
  await share.click();
  const area = page.locator("textarea").first();
  await expect(area).toBeVisible();
  expect(await area.inputValue()).toMatch(/^PP1\./);
});
