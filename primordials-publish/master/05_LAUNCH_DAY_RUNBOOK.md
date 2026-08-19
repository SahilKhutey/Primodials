# 🚀 PRIMODIALS — Launch Day Runbook (T-Minus Minute-by-Minute)

---

## ⏰ T-MINUS COUNTDOWN

### T-24 Hours (Monday Evening)
- [ ] Verify GitHub Release `v2.1.0` tag exists with all binary assets attached.
- [ ] Confirm `primordials.dev` domain DNS resolving cleanly through Cloudflare.
- [ ] Test itch.io playable browser demo on multiple browser engines (Chrome, Firefox, Safari).
- [ ] Verify all scheduled Buffer posts for Twitter and LinkedIn.
- [ ] Get 8 hours of restful sleep.

### T-30 Minutes (Tuesday 8:30 AM EST)
- [ ] Open second monitor with Plausible analytics and Discord.
- [ ] Load submission drafts in tabs:
  - Tab 1: Product Hunt Maker submission
  - Tab 2: Hacker News `Show HN: Primordials`
  - Tab 3: Twitter thread compose window
  - Tab 4: Subreddit drafts (`r/wallpaperengine`, `r/IndieGaming`)

### T-0 (9:01 AM EST — THE DROP)
1. **Product Hunt:** Hit "Publish", post Maker introduction comment immediately.
2. **Hacker News:** Submit *Show HN: Primordials — Living desktop ecosystem in C++23 & React*.
3. **Twitter / X:** Post 10-part launch thread with 60 FPS demo GIF.
4. **Email Newsletter:** Trigger launch announcement broadcast.

### T+30 Minutes to T+6 Hours (Active Engagement)
- [ ] Respond to all Hacker News technical questions regarding Lotka-Volterra math and ECS performance.
- [ ] Answer questions on Product Hunt within 15 minutes.
- [ ] Welcome first wave of Discord members in `#introductions`.
- [ ] Run dry-run email outreach for Tier 1 press outlets:
  ```bash
  python primordials-publish/marketing/scripts/email-outreach.py --tier 1 --dry-run
  ```
