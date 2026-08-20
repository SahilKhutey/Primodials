import { test, expect } from "./fixtures/app";

test("settings are reachable", async ({ page, appReady }) => {
  void appReady;
  const button = page.getByRole("button", { name: /settings/i }).first();
  await expect(button).toBeVisible();
  await button.click();
  await expect(page.getByText(/population/i).first()).toBeVisible();
});
