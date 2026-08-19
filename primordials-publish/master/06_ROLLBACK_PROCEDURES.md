# 🚨 PRIMODIALS — Emergency Rollback & Recovery Procedures

---

## 🛠️ SCENARIO 1: Critical Bug in Production Release

### Steps:
1. Revert to last stable commit on `main`:
   ```bash
   git revert HEAD
   git push origin main
   ```
2. Trigger the automated release workflow (`.github/workflows/release.yml`) or publish hotfix `v2.1.1`.
3. Post transparent update on Twitter and Discord:
   > *"We identified an edge-case crash affecting certain multi-monitor setups. Hotfix v2.1.1 is live now on GitHub and itch.io."*

---

## 🛠️ SCENARIO 2: Landing Page Deployment Issue (Vercel)

### Steps:
1. Roll back immediately using Vercel CLI:
   ```bash
   vercel rollback
   ```
2. Or in Vercel Web Dashboard: `Deployments` ➔ Select previous deployment ➔ `Promote to Production`.

---

## 🛠️ SCENARIO 3: Discord Server Raid / Spam Influx

### Steps:
1. Turn on Discord **Raid Mode** (Verification level set to High).
2. Issue bot purge command in affected channels: `/mod purge 50`.
3. Pause server invite links temporarily while triaging in `#mod-chat`.
