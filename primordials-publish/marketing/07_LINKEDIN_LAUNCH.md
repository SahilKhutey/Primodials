# 💼 PRIMODIALS — LinkedIn Launch & Engineering Thought Leadership

---

## 📝 LAUNCH POST (Day 9 — 10:00 AM EST)

I spent 6 months building a desktop ecosystem simulation that runs real Lotka-Volterra predator-prey dynamics at 60 frames per second.

It's free, open source under MIT, and built solo from Bilaspur, India.

Here is what I learned about systems architecture and indie engineering in 2026:

1. **Data-Oriented Design scales where OOP stalls.**  
Iterating over 250,000 objects in traditional object-oriented hierarchies caused catastrophic cache misses. Rewriting the core using Structure-of-Arrays (SoA) and archetype-based iteration allowed 8 entity positions to fit tightly in a single 64-byte L1 cache line — unlocking locked 60 FPS performance.

2. **Open Source forces architectural discipline.**  
Knowing anyone can audit the code pushes you to write comprehensive test suites (221 Catch2 tests, 1,311 assertions) and enforce strict determinism from day one.

3. **Building for delight beats building for deadlines.**  
Six months of asking *"What would make a desktop background truly feel alive?"* produced deeper technical breakthroughs than any arbitrary corporate timeline.

Check out the project:  
🔗 GitHub: https://github.com/SahilKhutey/Primodials  
🌐 Website: https://primordials.dev  

#indiedev #gamedev #softwareengineering #cpp #opensource #simulation
