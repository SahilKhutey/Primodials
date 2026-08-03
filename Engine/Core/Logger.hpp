#pragma once

#include "Core/Platform.hpp"
#include <string>
#include <format>
#include <utility>

namespace Shape {

enum class LogLevel : u8 {
    Trace = 0,
    Info,
    Warning,
    Error,
    Critical
};

class Logger {
public:
    static void Initialize(const char* logFilepath = nullptr);
    static void Shutdown();

    static void Log(LogLevel level, const char* file, i32 line, const std::string& message);

    template <typename... Args>
    static void LogFormat(LogLevel level, const char* file, i32 line, std::format_string<Args...> fmt, Args&&... args) {
        Log(level, file, line, std::format(fmt, std::forward<Args>(args)...));
    }
};

} // namespace Shape

// Logging Macros using C++20 __VA_OPT__ for clean standards compliance
#define SHAPE_LOG_TRACE(fmt, ...)    ::Shape::Logger::LogFormat(::Shape::LogLevel::Trace, __FILE__, __LINE__, fmt __VA_OPT__(,) __VA_ARGS__)
#define SHAPE_LOG_INFO(fmt, ...)     ::Shape::Logger::LogFormat(::Shape::LogLevel::Info, __FILE__, __LINE__, fmt __VA_OPT__(,) __VA_ARGS__)
#define SHAPE_LOG_WARN(fmt, ...)     ::Shape::Logger::LogFormat(::Shape::LogLevel::Warning, __FILE__, __LINE__, fmt __VA_OPT__(,) __VA_ARGS__)
#define SHAPE_LOG_ERROR(fmt, ...)    ::Shape::Logger::LogFormat(::Shape::LogLevel::Error, __FILE__, __LINE__, fmt __VA_OPT__(,) __VA_ARGS__)
#define SHAPE_LOG_CRITICAL(fmt, ...) ::Shape::Logger::LogFormat(::Shape::LogLevel::Critical, __FILE__, __LINE__, fmt __VA_OPT__(,) __VA_ARGS__)
