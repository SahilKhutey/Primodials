#include "Scripting/LuaModLoader.hpp"

namespace Shape::Scripting {

LuaModLoader::LuaModLoader(const std::string& modsDirectory)
    : m_modsDirectory(modsDirectory) {
    m_sandbox.Initialize();
}

bool LuaModLoader::LoadMod(const std::string& modId) {
    ModManifest manifest;
    manifest.id = modId;
    manifest.name = "Mod_" + modId;
    manifest.author = "Community";
    manifest.mainScript = m_modsDirectory + "/" + modId + "/mod.lua";

    if (m_sandbox.ExecuteString("mod_id = '" + modId + "'")) {
        m_loadedMods.push_back(manifest);
        return true;
    }
    return false;
}

usize LuaModLoader::LoadAllMods() {
    m_loadedMods.clear();
    LoadMod("custom_species");
    LoadMod("weather_events");
    return m_loadedMods.size();
}

} // namespace Shape::Scripting
