// Telemetry.cpp
#include "Telemetry.hpp"
#include "Core/Logger.hpp"
#include "Core/Platform.hpp"
#include "Utility/CRC32.hpp"

#include <chrono>
#include <fstream>
#include <random>
#include <sstream>

namespace PolygonalPrimordials {

    Telemetry& Telemetry::instance() {
        static Telemetry t;
        return t;
    }

    bool Telemetry::initialize(const std::string& /*config_path*/) {
        std::lock_guard lock(m_mutex);
        m_session_id = generate_session_id();
        m_initialized = true;
        SHAPE_LOG_INFO("Telemetry: Initialized (session {})", m_session_id);
        return true;
    }

    void Telemetry::shutdown() {
        flush();
        m_initialized = false;
    }

    void Telemetry::set_consent(TelemetryConsent c) {
        m_consent = c;
    }

    bool Telemetry::can_send() const {
        return m_initialized && m_consent == TelemetryConsent::Granted;
    }

    void Telemetry::event(const std::string& name, const TelemetryEvent& data) {
        if (!can_send()) return;
        std::lock_guard lock(m_mutex);
        // Serialize and queue
        std::string payload = serialize_event(name, data);
        send_to_endpoint(payload);
    }

    void Telemetry::event(const std::string& name, std::initializer_list<std::pair<std::string, TelemetryValue>> init) {
        if (!can_send()) return;
        TelemetryEvent ev;
        for (const auto& [k, v] : init) ev[k] = v;
        event(name, ev);
    }

    void Telemetry::screen(const std::string& screen_name) {
        event("screen_view", {{"screen", screen_name}});
    }

    void Telemetry::timing(const std::string& name, double ms) {
        event("timing", {{"name", name}, {"duration_ms", ms}});
    }

    void Telemetry::flush() {
        std::lock_guard lock(m_mutex);
        // Flush queue
    }

    void Telemetry::send_to_endpoint(const std::string& payload) {
        // HTTP POST stub
        SHAPE_LOG_TRACE("Telemetry: Sending: {}", payload);
    }

    std::string Telemetry::serialize_event(const std::string& name, const TelemetryEvent& /*data*/) const {
        std::stringstream ss;
        ss << "{\"event\":\"" << name << "\",\"session\":\"" << m_session_id << "\"}";
        return ss.str();
    }

    std::string Telemetry::generate_session_id() const {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<uint32_t> dis;
        std::stringstream ss;
        ss << std::hex << dis(gen) << "-" << dis(gen);
        return ss.str();
    }

} // namespace
