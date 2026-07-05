// LiveOps/src/SupportWorkflow.h
// Player support ticket system
#pragma once

#include <string>
#include <vector>
#include <chrono>
#include <optional>
#include <unordered_map>

namespace PolygonalPrimordials::LiveOps {

    enum class TicketStatus : uint8_t {
        New, InProgress, WaitingOnPlayer, Resolved, Closed, Reopened
    };

    enum class TicketCategory : uint8_t {
        Bug, Crash, Performance, Account, SaveGame, Modding,
        Multiplayer, Feature, Billing, Other
    };

    enum class TicketPriority : uint8_t {
        Low, Medium, High, Urgent
    };

    struct TicketMessage {
        std::string id;
        std::string author_id;
        std::string author_name;
        std::string body;
        std::vector<std::string> attachments;
        bool from_staff = false;
        std::chrono::system_clock::time_point sent_at;
    };

    struct Ticket {
        std::string id;
        std::string user_id;
        std::string subject;
        std::string body;
        std::vector<std::string> attachments;
        TicketStatus status = TicketStatus::New;
        TicketCategory category = TicketCategory::Other;
        TicketPriority priority = TicketPriority::Medium;
        std::vector<std::string> tags;
        std::string assigned_to;
        std::vector<TicketMessage> messages;
        std::chrono::system_clock::time_point created_at;
        std::chrono::system_clock::time_point updated_at;
        std::optional<int> satisfaction_rating;  // 1-5 after resolution
    };

    class SupportWorkflow {
    public:
        Ticket create(const std::string& user_id, const std::string& subject,
                      const std::string& body, TicketCategory cat = TicketCategory::Other);
        bool assign(const std::string& ticket_id, const std::string& staff_id);
        bool respond(const std::string& ticket_id, const std::string& body,
                     const std::string& staff_id, const std::string& staff_name);
        bool update_status(const std::string& ticket_id, TicketStatus status);
        bool add_tag(const std::string& ticket_id, const std::string& tag);
        bool rate(const std::string& ticket_id, int rating);

        // Queries
        Ticket* get(const std::string& id);
        std::vector<Ticket> open() const;
        std::vector<Ticket> assigned_to(const std::string& staff_id) const;
        std::vector<Ticket> by_status(TicketStatus status) const;
        std::vector<Ticket> by_category(TicketCategory cat) const;
        std::vector<Ticket> by_priority(TicketPriority p) const;
        std::vector<Ticket> search(const std::string& query) const;

        // Analytics
        struct SupportStats {
            int open_count = 0;
            int new_today = 0;
            int resolved_today = 0;
            double avg_resolution_hours = 0;
            double avg_satisfaction = 0;
            std::unordered_map<TicketCategory, int> by_category;
        };
        SupportStats stats() const;

        // Auto-response
        void set_canned_responses(const std::unordered_map<std::string, std::string>& responses);
        std::string get_canned(const std::string& key) const;

    private:
        std::string generate_id() const;
        TicketStatus auto_categorize(const std::string& body) const;
        TicketPriority auto_prioritize(const std::string& body) const;

        std::vector<Ticket> m_tickets;
        std::unordered_map<std::string, std::string> m_canned_responses;
    };

} // namespace
