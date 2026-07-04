#include "Core/Logger.hpp"
#include <iostream>
#include <fstream>
#include <mutex>
#include <chrono>
#include <filesystem>

#if defined(SHAPE_PLATFORM_WINDOWS)
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #include <windows.h>
#endif

namespace Shape {

namespace {
    std::mutex s_LogMutex;
    std::ofstream s_LogFile;
    bool s_Initialized = false;

    const char* LogLevelToString(LogLevel level) {
        switch (level) {
            case LogLevel::Trace:    return "TRACE";
            case LogLevel::Info:     return "INFO";
            case LogLevel::Warning:  return "WARN";
            case LogLevel::Error:    return "ERROR";
            case LogLevel::Critical: return "FATAL";
        }
        return "UNKNOWN";
    }

    const char* LogLevelToColor(LogLevel level) {
        switch (level) {
            case LogLevel::Trace:    return "\033[90m"; // Gray
            case LogLevel::Info:     return "\033[32m"; // Green
            case LogLevel::Warning:  return "\033[33m"; // Yellow
            case LogLevel::Error:    return "\033[31m"; // Red
            case LogLevel::Critical: return "\033[35;1m"; // Bold Magenta
        }
        return "\033[0m";
    }

    const char* ColorReset() {
        return "\033[0m";
    }
}

void Logger::Initialize(const char* logFilepath) {
    std::lock_guard<std::mutex> lock(s_LogMutex);
    if (s_Initialized) return;

#if defined(SHAPE_PLATFORM_WINDOWS)
    // Enable ANSI escape codes on Windows Console
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode)) {
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, dwMode);
        }
    }
#endif

    if (logFilepath) {
        std::filesystem::path path(logFilepath);
        if (path.has_parent_path()) {
            std::filesystem::create_directories(path.parent_path());
        }
        s_LogFile.open(logFilepath, std::ios::out | std::ios::app);
    }
    s_Initialized = true;
}

void Logger::Shutdown() {
    std::lock_guard<std::mutex> lock(s_LogMutex);
    if (!s_Initialized) return;

    if (s_LogFile.is_open()) {
        s_LogFile.close();
    }
    s_Initialized = false;
}

void Logger::Log(LogLevel level, const char* file, i32 line, const std::string& message) {
    std::lock_guard<std::mutex> lock(s_LogMutex);

    // Get current time formatted
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    std::tm tm_now;
#if defined(SHAPE_PLATFORM_WINDOWS)
    localtime_s(&tm_now, &time_t_now);
#else
    localtime_r(&time_t_now, &tm_now);
#endif

    char timeBuffer[24];
    std::strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", &tm_now);

    // Strip path to only show file basename
    std::filesystem::path filePath(file);
    std::string fileBasename = filePath.filename().string();

    // Format Console Output: [TIMESTAMP] [LEVEL] [file:line] message
    std::string consoleOut = std::format(
        "[{}] {}{:<5}{} [{}:{}] {}\n",
        timeBuffer,
        LogLevelToColor(level),
        LogLevelToString(level),
        ColorReset(),
        fileBasename,
        line,
        message
    );

    std::cout << consoleOut << std::flush;

    // Log to file if open (no color codes)
    if (s_LogFile.is_open()) {
        std::string fileOut = std::format(
            "[{}] [{:<5}] [{}:{}] {}\n",
            timeBuffer,
            LogLevelToString(level),
            fileBasename,
            line,
            message
        );
        s_LogFile << fileOut << std::flush;
    }
}

} // namespace Shape
