// Bots/Discord/src/Commands.cpp
#include "Commands.h"
// #include <dpp/dpp.h> (mock)

namespace dpp { 
    struct slashcommand_t {}; 
    class cluster {}; 
}

namespace PolygonalPrimordials::Bots::Discord {
    Commands::Commands(dpp::cluster& bot) : m_bot(bot) {
        m_handlers["status"] = [this](const dpp::slashcommand_t& e) { cmd_status(e); };
        m_handlers["link"] = [this](const dpp::slashcommand_t& e) { cmd_link(e); };
        m_handlers["stats"] = [this](const dpp::slashcommand_t& e) { cmd_stats(e); };
        m_handlers["ticket"] = [this](const dpp::slashcommand_t& e) { cmd_ticket(e); };
        m_handlers["wiki"] = [this](const dpp::slashcommand_t& e) { cmd_wiki(e); };
        m_handlers["mod_ban"] = [this](const dpp::slashcommand_t& e) { cmd_mod_ban(e); };
        m_handlers["mod_warn"] = [this](const dpp::slashcommand_t& e) { cmd_mod_warn(e); };
    }
    void Commands::register_commands() {}
    void Commands::handle_interaction(const dpp::slashcommand_t&) {}
    void Commands::cmd_status(const dpp::slashcommand_t&) {}
    void Commands::cmd_link(const dpp::slashcommand_t&) {}
    void Commands::cmd_stats(const dpp::slashcommand_t&) {}
    void Commands::cmd_ticket(const dpp::slashcommand_t&) {}
    void Commands::cmd_wiki(const dpp::slashcommand_t&) {}
    void Commands::cmd_mod_ban(const dpp::slashcommand_t&) {}
    void Commands::cmd_mod_warn(const dpp::slashcommand_t&) {}
}
