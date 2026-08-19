# 🌐 PRIMODIALS — WebGPU Browser Compatibility Matrix

---

## 🖥️ BROWSER SUPPORT TABLE (2026)

| Browser | Version | WebGPU Status | Native Driver Layer | Default Tier |
| :--- | :--- | :--- | :--- | :--- |
| **Google Chrome** | 113+ | ✅ Fully Supported | D3D12 / Vulkan / Metal | Tier 1 (WebGPU) |
| **Microsoft Edge** | 113+ | ✅ Fully Supported | D3D12 / Vulkan | Tier 1 (WebGPU) |
| **Mozilla Firefox** | 121+ | ✅ Fully Supported | D3D12 / Vulkan / Metal | Tier 1 (WebGPU) |
| **Apple Safari (macOS)** | 17+ | ✅ Fully Supported | Metal | Tier 1 (WebGPU) |
| **Apple Safari (iOS/iPadOS)**| 17+ | ✅ Fully Supported | Metal | Tier 1 (WebGPU) |
| **Brave / Opera / Vivaldi** | Modern | ✅ Fully Supported | D3D12 / Vulkan / Metal | Tier 1 (WebGPU) |
| **Chrome (Legacy)** | 90–112 | ❌ Not Supported | WebGL2 | Tier 2 (WebGL2) |
| **Safari (Legacy)** | 14–16 | ❌ Not Supported | WebGL2 | Tier 2 (WebGL2) |
| **Internet Explorer / Old** | Any | ❌ Not Supported | Canvas 2D | Tier 3 (Canvas) |

---

## 📱 HARDWARE COMPATIBILITY REQUIREMENTS

- **Windows:** Windows 10 (Build 19041+) or Windows 11 with DirectX 12 Feature Level 11_1+ or Vulkan 1.1+.
- **macOS:** macOS 13 (Ventura) or macOS 14+ with Apple Silicon (M1/M2/M3/M4) or AMD Radeon GPUs supporting Metal 3.
- **Linux:** Kernel 5.15+ with Mesa 23.0+ (RADV/ANV Vulkan drivers).
- **Mobile (iOS / Android):** iOS 17+ (Apple A12 Bionic+) / Android 12+ (Qualcomm Adreno 6xx+ / ARM Mali-G7x+).
