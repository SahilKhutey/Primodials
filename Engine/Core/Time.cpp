#include "Core/Time.hpp"

#if SHAPE_LOGGING_ENABLED
#include "Core/Logger.hpp"
#include <format>
#endif

namespace Shape::Time {

ScopeTimer::~ScopeTimer() noexcept {
#if SHAPE_LOGGING_ENABLED
    const i64 us = ElapsedUs();
    SHAPE_LOG_TRACE("{} took {} us", m_name, us);
#endif
}

} // namespace Shape::Time
