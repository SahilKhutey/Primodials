#pragma once

/// @file StringUtils.hpp
/// @brief String manipulation utilities — trimming, splitting, conversion.
///
/// All functions operate on std::string_view or std::string.
/// No heap allocations for trim/contains/starts_with/ends_with.

#include <algorithm>
#include <charconv>
#include <cstring>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

namespace Shape::StringUtils {

// ─── Comparison ──────────────────────────────────────────────────────────────

inline bool EqualsIgnoreCase(std::string_view a, std::string_view b) noexcept {
    if (a.size() != b.size()) return false;
    for (usize i = 0; i < a.size(); ++i) {
        if (std::tolower(static_cast<unsigned char>(a[i])) !=
            std::tolower(static_cast<unsigned char>(b[i]))) return false;
    }
    return true;
}

// ─── Search ───────────────────────────────────────────────────────────────────

inline bool Contains(std::string_view haystack, std::string_view needle) noexcept {
    return haystack.find(needle) != std::string_view::npos;
}

inline bool StartsWith(std::string_view s, std::string_view prefix) noexcept {
    return s.size() >= prefix.size() && s.compare(0, prefix.size(), prefix) == 0;
}

inline bool EndsWith(std::string_view s, std::string_view suffix) noexcept {
    return s.size() >= suffix.size() &&
           s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0;
}

// ─── Trimming ─────────────────────────────────────────────────────────────────

inline std::string_view TrimLeft(std::string_view s) noexcept {
    usize i = 0;
    while (i < s.size() && std::isspace(static_cast<unsigned char>(s[i]))) ++i;
    return s.substr(i);
}

inline std::string_view TrimRight(std::string_view s) noexcept {
    usize n = s.size();
    while (n > 0 && std::isspace(static_cast<unsigned char>(s[n - 1]))) --n;
    return s.substr(0, n);
}

inline std::string_view Trim(std::string_view s) noexcept {
    return TrimRight(TrimLeft(s));
}

// ─── Splitting ────────────────────────────────────────────────────────────────

inline std::vector<std::string_view> Split(std::string_view s, char delim) {
    std::vector<std::string_view> out;
    usize start = 0;
    for (usize i = 0; i <= s.size(); ++i) {
        if (i == s.size() || s[i] == delim) {
            out.push_back(s.substr(start, i - start));
            start = i + 1;
        }
    }
    return out;
}

inline std::vector<std::string_view> Split(std::string_view s, std::string_view delim) {
    std::vector<std::string_view> out;
    usize pos = 0;
    for (;;) {
        const auto next = s.find(delim, pos);
        if (next == std::string_view::npos) {
            out.push_back(s.substr(pos));
            break;
        }
        out.push_back(s.substr(pos, next - pos));
        pos = next + delim.size();
    }
    return out;
}

// ─── Joining ──────────────────────────────────────────────────────────────────

inline std::string Join(std::span<const std::string_view> parts, std::string_view delim) {
    std::string out;
    for (usize i = 0; i < parts.size(); ++i) {
        if (i > 0) out.append(delim);
        out.append(parts[i]);
    }
    return out;
}

inline std::string Join(std::span<const std::string> parts, std::string_view delim) {
    std::string out;
    for (usize i = 0; i < parts.size(); ++i) {
        if (i > 0) out.append(delim);
        out.append(parts[i]);
    }
    return out;
}

// ─── Replacement ──────────────────────────────────────────────────────────────

inline std::string Replace(std::string_view s, std::string_view from, std::string_view to) {
    std::string out;
    out.reserve(s.size());
    usize pos = 0;
    for (;;) {
        const auto next = s.find(from, pos);
        if (next == std::string_view::npos) {
            out.append(s.substr(pos));
            break;
        }
        out.append(s.substr(pos, next - pos));
        out.append(to);
        pos = next + from.size();
    }
    return out;
}

// ─── Case conversion ─────────────────────────────────────────────────────────

inline std::string ToLower(std::string_view s) {
    std::string out(s);
    for (auto& c : out)
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return out;
}

inline std::string ToUpper(std::string_view s) {
    std::string out(s);
    for (auto& c : out)
        c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    return out;
}

// ─── Type conversion ─────────────────────────────────────────────────────────

/// Parse a numeric value from a string. Returns nullopt on failure.
template<typename T>
inline std::optional<T> Parse(std::string_view s) noexcept {
    T value{};
    const char* begin = s.data();
    const char* end   = s.data() + s.size();
    auto [ptr, ec] = std::from_chars(begin, end, value);
    if (ec != std::errc{} || ptr != end) return std::nullopt;
    return value;
}

} // namespace Shape::StringUtils
