# Contributing to Primordials

🎉 **Thank you for considering contributing to Primordials!**  
Primordials is an independent open-source project, and every contribution — code, documentation, bug reports, or new biomes — is deeply appreciated.

---

## 📜 Code of Conduct

This project follows the [Contributor Covenant Code of Conduct](CODE_OF_CONDUCT.md). By participating, you agree to uphold it. Please report unacceptable behavior to **conduct@primordials.dev**.

---

## 🐛 Reporting Bugs

Bugs are tracked as [GitHub Issues](https://github.com/SahilKhutey/Primodials/issues).

**Before filing:**
1. Check the issue tracker to verify the bug hasn't already been reported.
2. Ensure you are testing against the latest `main` branch.
3. Isolate the issue with minimal reproduction steps.

Please use the [Bug Report Template](.github/ISSUE_TEMPLATE/bug_report.md).

---

## 💡 Suggesting Features

Feature suggestions are tracked via the [Feature Request Template](.github/ISSUE_TEMPLATE/feature_request.md). Please include the problem context, proposed solution, and alternative ideas considered.

---

## 🌿 Contributing a Biome

Biomes are the easiest and most creative way to contribute. You do not need deep programming experience — you can create and submit a new ecosystem JSON config.  
See **[docs/CONTRIBUTING_BIOMES.md](docs/CONTRIBUTING_BIOMES.md)** for specifications.

---

## 🛠️ Submitting Code

### Workflow

```bash
# 1. Fork and clone the repository
git clone https://github.com/YOUR-USERNAME/Primodials.git
cd Primodials

# 2. Set up development environment
./dev/setup_dev.sh

# 3. Create a feature branch
git checkout -b feature/my-new-biome

# 4. Make changes and run tests
npm run lint
npm run typecheck
./build_all.sh engine && ctest --test-dir build/test --output-on-failure

# 5. Commit using Conventional Commits
git commit -m "feat(biome): add hydrothermal vent ecosystem"

# 6. Push and create Pull Request
git push origin feature/my-new-biome
```

---

## 📐 Coding Standards

### TypeScript / Web App (`src/`)
- ESLint + Prettier rules enforced
- Strict typing enabled (`noImplicitAny`, no raw `any`)
- Functional React components with custom hooks
- Tailwind CSS utility classes adhering to `brand/COLOR_TOKENS.md`

### C++ Engine (`Engine/`, `Games/`)
- Modern C++23 standard
- RAII for all memory management (no raw pointers or manual `new`/`delete`)
- Formatting matching `.clang-format`
- 100% test pass rate with Catch2

---

## 🏷️ Commit Message Format

We follow the [Conventional Commits](https://www.conventionalcommits.org/) specification:
`feat:`, `fix:`, `docs:`, `style:`, `refactor:`, `perf:`, `test:`, `chore:`
