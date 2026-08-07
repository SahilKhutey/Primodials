# 🛡️ Security Hardening & Protection Specification

This document details the comprehensive security hardening features, binary defenses, sandboxing, path traversal protection, and installer security controls implemented in **Primordials**.

---

## 1. ⚙️ Binary & Compiler Hardening Flags (`cmake/SecurityHardening.cmake`)

Every executable and static/shared library target (`ShapeEngine`, `Primordials`, `PrimordialsWallpaper`) is built with production-grade binary defenses enabled:

### Windows (MSVC)
- **`/GS` (Buffer Security Check)**: Inserts security cookie / stack canary to detect stack buffer overflows.
- **`/guard:cf` (Control Flow Guard)**: Enables CFI (Control Flow Integrity) to prevent indirect call hijacking.
- **`/DYNAMICBASE` (ASLR)**: Enables 64-bit Address Space Layout Randomization.
- **`/NXCOMPAT` (DEP)**: Enforces Data Execution Prevention (Non-Executable stack and heap memory).
- **`/HIGHENTROPYVA`**: Enables 64-bit high entropy ASLR address space (8 TB random offset range).
- **`/sdl`**: Enables mandatory Security Development Lifecycle warnings and static analysis checks.

### Linux & macOS (GCC / Clang)
- **`-fstack-protector-strong`**: Emits strong canary protection for functions with character arrays or local reference pointers.
- **`-D_FORTIFY_SOURCE=2`**: Replaces risky standard library string/memory functions with buffer-length checked variants.
- **`-fPIE` & `-pie`**: Compiles position-independent executables for full ASLR support.
- **`-Wl,-z,relro,-z,now`**: Enforces Read-Only Relocations (RELRO) and immediate binding to prevent GOT (Global Offset Table) overwrite attacks.
- **`-Wl,-z,noexecstack`**: Prohibits stack memory execution (NX bit enforcement).

---

## 2. 🔒 Scripting & Mod Sandboxing (Lua Environment)

- **Library Blacklisting**: Standard `io`, `os`, and `debug` libraries are stripped from the execution state (`io = nil`, `os = nil`, `debug = nil`).
- **Memory Quotas**: Scripting allocations are monitored and hard-capped to prevent OOM crash vectors.
- **Instruction Execution Limits**: Step hooks enforce a maximum instruction budget per frame to prevent infinite loop denial of service.

---

## 3. 📂 Path Traversal & File I/O Safety

- **Canonical Path Resolution**: User-provided file paths for modding, themes, and save loading are sanitized via `std::filesystem::canonical` / `weakly_canonical`.
- **Directory Traversal Prevention**: Path components containing `../` or relative root escapes outside the target sandbox directory (`Content/` or `%APPDATA%/Primordials/`) are rejected immediately with log warnings.

---

## 4. 🍎 macOS Hardened Runtime (`installer/macos/entitlements.plist`)

- Enforces macOS Gatekeeper Hardened Runtime compliance.
- Restricts camera/microphone access (`com.apple.security.device.camera = false`).
- Restricts local user file access strictly to user-selected paths (`user-selected.read-write`).

---

## 5. 📦 Release Checksums & Packaging Integrity

- All release artifacts produced by `create_release.bat` & `tools/package_installer.py` generate SHA256 cryptographic digests saved in `dist/SHA256SUMS`.
- Tamper verification is automatically enforced during release gate checks (`gate.sh` / `gate.bat`).
