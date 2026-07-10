// Telemetry.h
#pragma once
#include "Core/NonCopyable.hpp"
#include <string>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <variant>

namespace PolygonalPrimordials {

    using TelemetryValue = std::variant<int64_t, double, bool, std::string>;
    using TelemetryEvent = std::unordered_map<std::string, TelemetryValue>;

    enum class TelemetryConsent : uint8_t {
        NotAsked,
        Granted,
        Denied,
        GrantedEssentialOnly  // Anonymous aggregates only
    };

    class Telemetry : public Shape::NonCopyable {
    public:
        static Telemetry& instance();

        bool initialize(const std::string& config_path);
        void shutdown();

        // Consent (GDPR)
        void set_consent(TelemetryConsent c);
        TelemetryConsent consent() const { return m_consent; }
        bool can_send() const;

        // Events
        void event(const std::string& name, const TelemetryEvent& data = {});
        void event(const std::string& name, std::initializer_list<std::pair<std::string, TelemetryValue>> init);
        void screen(const std::string& screen_name);
        void timing(const std::string& name, double ms);

        // Flush
        void flush();

        // Privacy
        void anonymize_ip(bool v) { m_anonymize_ip = v; }
        void opt_out_all() { m_consent = TelemetryConsent::Denied; }

    private:
        Telemetry() = default;
        void send_to_endpoint(const std::string& payload);
        std::string serialize_event(const std::string& name, const TelemetryEvent& data) const;
        std::string generate_session_id() const;

        std::mutex m_mutex;
        std::string m_endpoint;
        std::string m_session_id;
        std::string m_user_id;  // anonymous UUID
        TelemetryConsent m_consent = TelemetryConsent::NotAsked;
        bool m_anonymize_ip = true;
        bool m_initialized = false;
    };

} // namespace
