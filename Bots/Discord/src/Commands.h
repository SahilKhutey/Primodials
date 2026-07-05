// Bots/Discord/src/Commands.h
// Slash command handler for Discord Bot
#pragma once

#include <string>
#include <vector>
#include <functional>
#include <unordered_map>

// Forward-declare Discord library types to avoid huge includes in headers
namespace dpp {
    struct slashcommand_t;
    class cluster;
}

namespace PolygonalPrimordials::Bots::Discord {

    class Commands {
    public:
        Commands(dpp::cluster& bot);

        // Register all standard commands to Discord API
        void register_commands();

        // Handle incoming interaction
        void handle_interaction(const dpp::slashcommand_t& event);

    private:
        // Command implementations
        void cmd_status(const dpp::slashcommand_t& event);
        void cmd_link(const dpp::slashcommand_t& event);
        void cmd_stats(const dpp::slashcommand_t& event);
        void cmd_ticket(const dpp::slashcommand_t& event);
        void cmd_wiki(const dpp::slashcommand_t& event);
        void cmd_mod_ban(const dpp::slashcommand_t& event);
        void cmd_mod_warn(const dpp::slashcommand_t& event);

        dpp::cluster& m_bot;
        std::unordered_map<std::string, std::function<void(const dpp::slashcommand_t&)>> m_handlers;
    };

} // namespace
