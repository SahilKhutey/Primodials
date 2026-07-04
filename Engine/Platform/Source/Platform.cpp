#include "ShapeEngine/Platform/Platform.hpp"
#include "ShapeEngine/Platform/FileSystem.hpp"
#include "ShapeEngine/Platform/Clock.hpp"
#include "ShapeEngine/Platform/SystemInfo.hpp"
#include "ShapeEngine/Platform/Clipboard.hpp"
#include "ShapeEngine/Platform/Monitor.hpp"
#include "ShapeEngine/Platform/Process.hpp"
#include "ShapeEngine/Platform/DynamicLibrary.hpp"
#include "Core/Logger.hpp"

#include <chrono>
#include <cstdlib>

#if defined(_WIN32) || defined(_WIN64)
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #include <windows.h>
    #undef CreateDirectory
#else
    #include <unistd.h>
    #include <dlfcn.h>
    #include <sys/sysinfo.h>
#endif

namespace Shape {

// ── Platform ─────────────────────────────────────────────────────────────────
bool Platform::Initialize() {
    SHAPE_LOG_INFO("Platform Layer Initialized successfully.");
    return true;
}

void Platform::Shutdown() {
    SHAPE_LOG_INFO("Platform Layer Shutdown complete.");
}

// ── FileSystem ───────────────────────────────────────────────────────────────
bool FileSystem::Exists(const std::filesystem::path& path) {
    return std::filesystem::exists(path);
}

bool FileSystem::CreateDirectory(const std::filesystem::path& path) {
    return std::filesystem::create_directories(path);
}

bool FileSystem::Remove(const std::filesystem::path& path) {
    return std::filesystem::remove_all(path) > 0;
}

// ── Clock ────────────────────────────────────────────────────────────────────
f64 Clock::Seconds() {
    auto now = std::chrono::high_resolution_clock::now().time_since_epoch();
    return std::chrono::duration<f64>(now).count();
}

f64 Clock::Milliseconds() {
    auto now = std::chrono::high_resolution_clock::now().time_since_epoch();
    return std::chrono::duration<f64, std::milli>(now).count();
}

// ── SystemInfo ───────────────────────────────────────────────────────────────
CPUInfo SystemInfo::GetCPUInfo() {
    CPUInfo info{};
    info.AVX2 = true; // Default standard architectures
    info.SSE42 = true;

#if defined(_WIN32) || defined(_WIN64)
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    info.LogicalCores = sysInfo.dwNumberOfProcessors;

    MEMORYSTATUSEX memStatus;
    memStatus.dwLength = sizeof(memStatus);
    GlobalMemoryStatusEx(&memStatus);
    info.RAM = memStatus.ullTotalPhys;
#else
    info.LogicalCores = sysconf(_SC_NPROCESSORS_ONLN);
    struct sysinfo si;
    if (sysinfo(&si) == 0) {
        info.RAM = si.totalram * si.mem_unit;
    } else {
        info.RAM = 16ULL * 1024 * 1024 * 1024; // 16GB fallback
    }
#endif
    return info;
}

// ── Clipboard ────────────────────────────────────────────────────────────────
static std::string g_ClipboardPlaceholder = "";

void Clipboard::SetText(const std::string& text) {
#if defined(_WIN32) || defined(_WIN64)
    if (OpenClipboard(nullptr)) {
        EmptyClipboard();
        HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, text.size() + 1);
        if (hg) {
            memcpy(GlobalLock(hg), text.c_str(), text.size() + 1);
            GlobalUnlock(hg);
            SetClipboardData(CF_TEXT, hg);
        }
        CloseClipboard();
    }
#endif
    g_ClipboardPlaceholder = text;
}

std::string Clipboard::GetText() {
#if defined(_WIN32) || defined(_WIN64)
    std::string result = "";
    if (OpenClipboard(nullptr)) {
        HANDLE hData = GetClipboardData(CF_TEXT);
        if (hData) {
            char* pszText = static_cast<char*>(GlobalLock(hData));
            if (pszText) {
                result = pszText;
            }
            GlobalUnlock(hData);
        }
        CloseClipboard();
        if (!result.empty()) {
            return result;
        }
    }
#endif
    return g_ClipboardPlaceholder;
}

// ── Monitor ──────────────────────────────────────────────────────────────────
MonitorInfo Monitor::Primary() {
    MonitorInfo info{};
#if defined(_WIN32) || defined(_WIN64)
    info.Width = GetSystemMetrics(SM_CXSCREEN);
    info.Height = GetSystemMetrics(SM_CYSCREEN);
    info.DPI = 96.0f; // Windows standard base scale
#else
    info.Width = 1920;
    info.Height = 1080;
    info.DPI = 96.0f;
#endif
    return info;
}

// ── Process ──────────────────────────────────────────────────────────────────
u32 Process::ID() {
#if defined(_WIN32) || defined(_WIN64)
    return GetCurrentProcessId();
#else
    return getpid();
#endif
}

void Process::Sleep(u32 milliseconds) {
#if defined(_WIN32) || defined(_WIN64)
    ::Sleep(milliseconds);
#else
    usleep(milliseconds * 1000);
#endif
}

void Process::Exit(i32 code) {
    std::exit(code);
}

// ── DynamicLibrary ───────────────────────────────────────────────────────────
DynamicLibrary::~DynamicLibrary() {
    Unload();
}

bool DynamicLibrary::Load(const char* path) {
    Unload();
#if defined(_WIN32) || defined(_WIN64)
    m_handle = LoadLibraryA(path);
#else
    m_handle = dlopen(path, RTLD_NOW);
#endif
    return m_handle != nullptr;
}

void* DynamicLibrary::GetSymbol(const char* name) {
    if (!m_handle) return nullptr;
#if defined(_WIN32) || defined(_WIN64)
    return reinterpret_cast<void*>(GetProcAddress(static_cast<HMODULE>(m_handle), name));
#else
    return dlsym(m_handle, name);
#endif
}

void DynamicLibrary::Unload() {
    if (m_handle) {
#if defined(_WIN32) || defined(_WIN64)
        FreeLibrary(static_cast<HMODULE>(m_handle));
#else
        dlclose(m_handle);
#endif
        m_handle = nullptr;
    }
}

} // namespace Shape
