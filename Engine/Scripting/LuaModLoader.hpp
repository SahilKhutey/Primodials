#pragma once

#include "Scripting/LuaSandbox.hpp"
#include <string>
#include <vector>
#include <memory>

namespace Shape::Scripting {

struct ModManifest {
    std::string id;
    std::string name;
    std::string version = "1.0.0";
    std::string author;
    std::string mainScript = "mod.lua";
    std::vector<std::string> dependencies;
    bool enabled = true;
};

class LuaModLoader {
public:
    explicit LuaModLoader(const std::string& modsDirectory = "Content/Mods");
    ~LuaModLoader() = default;

    bool LoadMod(const std::string& modId);
    usize LoadAllMods();

    const std::vector<ModManifest>& GetLoadedMods() const { return m_loadedMods; }
    LuaSandbox& GetSandbox() { return m_sandbox; }

private:
    std::string m_modsDirectory;
    LuaSandbox m_sandbox;
    std::vector<ModManifest> m_loadedMods;
};

} // namespace Shape::Scripting
