# Primordials — Revenue Launch Plan

Corrects and sharpens `production-development-plan.md` §6 with current, sourced platform
economics. The development work is done (all 25 test cases verified) — this is specifically
about the path from "finished software" to "actual income," researched fresh rather than
assumed.

---

## 1. The Correction That Changes the Plan

**Wallpaper Engine Workshop is not a revenue channel for ordinary creators.** Confirmed
directly by Wallpaper Engine's own developer on their Steam forum: *"No, this is not possible,
at least not through Steam. If this is something you want to do via Patreon or other ways of
directly receiving money, it's certainly an option."* There's a "curated" monetization program
mentioned in patch notes, but it's developer-selected, not something a solo creator can opt
into by submitting normally.

This doesn't remove Workshop from the plan — it just changes *what it's for*. It's still the
right free-distribution, audience-building move from the production plan. It just isn't where
money comes from.

**Where money actually comes from: a separate paid listing**, on a platform built for exactly
that. Two real options, researched fresh:

---

## 2. itch.io — The Recommended First Revenue Channel

| Fact | Detail |
|---|---|
| Listing cost | $0 |
| Revenue share | You set it, 0-100%, default 10% — the most creator-favorable split of any major platform |
| Payment processing | ~2.9% + $0.30/transaction (Stripe/PayPal, unavoidable, same on every platform) |
| Time to live | Same day — no review queue, no waiting period |
| Pricing model | "Pay what you want" available — reduces purchase friction, itch.io's own data shows it increases average payment ~30% over a fixed price |
| Fit for this product | **Strong** — itch.io gives HTML5/WebGL projects instant-play-in-browser support. Since Primordials is already a web app, this isn't a workaround, it's a natural fit: a visitor can *try the actual simulation in their browser* before deciding to buy the wallpaper build |

**Concrete pricing recommendation:** list at $4.99 with "pay what you want, $4.99 minimum" —
matches Wallpaper Engine's own $4.99 price point (a number 215,000+ buyers have already shown
they're comfortable with for this exact category), while PWYW pricing captures upside from
generous buyers without adding friction for price-sensitive ones.

**What to actually build for the listing** (none of this is new code — it's packaging what
already exists):
1. An itch.io project page with the browser-playable HTML5 build (`dist/` or `dist-wallpaper/`
   — the browser demo can run the interactive Simulation View, not the wallpaper-only build,
   since that's the more compelling "try before you buy" experience)
2. A separate downloadable ZIP of the `dist-wallpaper/` build as the actual purchasable product
   for people who want it as a real desktop background (paired with simple setup instructions,
   or eventually the Tauri-wrapped standalone app from the production plan's §5.2)
3. Screenshots (the real, verified simulation screenshots already captured in this session —
   **not** the AI-generated `Marketing/screenshots/*.jpg`, which the earlier repo analysis
   confirmed don't depict the actual product)
4. A short description leading with what's true and verifiable: a living ecosystem simulation
   that evolves — species, genetics, neural-network-driven behavior — running as your desktop
   background

---

## 3. Steam Direct — The Later, Bigger Channel

| Fact | Detail |
|---|---|
| Listing cost | $100 one-time, per app — **recoupable** once the app earns $1,000 in revenue |
| Revenue share | Valve takes 30% (drops to 25% past $10M, 20% past $50M — not relevant at this scale, but real) |
| Time to live | 4-6 weeks minimum: identity/tax verification, a mandatory 30-day wait after paying the fee, store page must be live 2+ weeks before release |
| Requirements | An existing Steam account with $5+ spent, tax questionnaire, bank details, no business entity required (can publish as an individual) |
| Fit for this product | Good, but only worth the $100 + weeks of lead time *after* itch.io has shown real demand — this matches the production plan's original sequencing recommendation, now with concrete numbers behind it |

**Not recommended yet.** This is the "Milestone 3" move from the production plan — worth doing
once itch.io sales or Workshop traction give a real signal, not before.

---

## 4. Revised Revenue Sequencing

This replaces the plan's earlier, more general Milestone framing with the concrete channel
research above:

**Step 1 (this week — no new code needed):** Wallpaper Engine Workshop submission — free
exposure, builds the "as seen by X users" credibility that helps every later paid listing.

**Step 2 (same week, in parallel):** itch.io listing — the actual revenue channel. $0 to list,
live the same day, and the browser-playable HTML5 build is a genuine differentiator this
product already has for free.

**Step 3 (only after Step 1/2 show real signal — favorites, downloads, sales):** Steam Direct,
if and only if the numbers justify the $100 + 4-6 week lead time.

**Not recommended at any point:** relying on Wallpaper Engine Workshop's payment system
directly — it doesn't meaningfully exist for a creator in this position, per §1.

---

## 5. Realistic Expectations, Stated Plainly

This remains true from the production plan and doesn't change with better channel research:
this is solo-dev side income, not a venture-scale outcome. itch.io's own numbers (a $9.99 game
at 100 monthly sales, 10% platform share) land in roughly $800-900/month territory *at that
sales volume* — and 100 monthly sales for a first release with no existing audience is an
optimistic, not baseline, assumption. The honest range for a first month is likely single or
low double-digit sales. What itch.io's low-friction, zero-cost listing actually buys is a real,
fast, cheap way to find out — not a guarantee.

---

## 6. Immediate Action Items

- [x] Package a browser-playable build of the interactive Simulation View for itch.io's HTML5 embed (`npm run package:itch` -> `dist-itch/primordials-browser-demo.zip`)
- [x] Package `dist-wallpaper/` as a downloadable ZIP for the actual purchasable product (`npm run package:itch` -> `dist-itch/primordials-wallpaper-package.zip`)
- [x] Package `dist-wallpaper-engine/` for Wallpaper Engine Workshop upload (`npm run package:wallpaper`)
- [ ] Create an itch.io account (free) and start a new project page
- [ ] Write the store description using verified facts from this session (real screenshots,
      real feature list) — not the AI-generated marketing assets flagged as inaccurate earlier
- [ ] Set pricing: $4.99 minimum, "pay what you want" enabled
- [ ] In parallel: complete the Wallpaper Engine Workshop submission steps already documented
      in `production-development-plan.md` §5.1 and the launch checklist
- [ ] Revisit Steam Direct only after the above shows real signal
