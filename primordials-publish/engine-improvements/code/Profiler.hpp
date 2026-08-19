/**
 * Primordials — Built-in RAII Scoped Profiler (C++23)
 *
 * Microsecond accurate performance diagnostics.
 *
 * License: MIT
 */

#pragma once

#include <chrono>
#include <cstdio>
#include <mutex>
#include <string>
#include <string_view>
#include <unordered_map>

namespace primordials::profile {

struct ProfileEntry {
    std::string name;
    std::uint64_t call_count = 0;
    std::chrono::nanoseconds total_time{};
};

class Profiler {
public:
    static Profiler& instance() noexcept {
        static Profiler p;
        return p;
    }

    void record(std::string_view name, std::chrono::nanoseconds duration) noexcept {
        std::lock_guard lock(mutex_);
        auto& entry = entries_[std::string{name}];
        entry.name = std::string{name};
        entry.call_count++;
        entry.total_time += duration;
    }

    void print_summary() const {
        std::lock_guard lock(mutex_);
        std::printf("\n=== PRIMORDIALS ENGINE PROFILER REPORT ===\n");
        std::printf("%-28s %10s %14s\n", "System Scope", "Invocations", "Total Time (ms)");
        std::printf("%-28s %10s %14s\n", "------------", "-----------", "---------------");
        for (const auto& [name, entry] : entries_) {
            const double ms = std::chrono::duration<double, std::milli>(entry.total_time).count();
            std::printf("%-28s %10llu %14.3f\n", name.c_str(), static_cast<unsigned long long>(entry.call_count), ms);
        }
        std::printf("==========================================\n\n");
    }

private:
    mutable std::mutex mutex_;
    std::unordered_map<std::string, ProfileEntry> entries_;
};

class ScopedTimer {
public:
    explicit ScopedTimer(std::string_view name) noexcept
        : name_(name), start_(std::chrono::high_resolution_clock::now()) {}

    ~ScopedTimer() {
        const auto end = std::chrono::high_resolution_clock::now();
        Profiler::instance().record(name_, end - start_);
    }

private:
    std::string_view name_;
    std::chrono::high_resolution_clock::time_point start_;
};

#define PROFILE_SCOPE(name) primordials::profile::ScopedTimer _timer_##__LINE__(name)

} // namespace primordials::profile
