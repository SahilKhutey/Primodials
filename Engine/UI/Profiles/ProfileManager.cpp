// Engine/UI/Profiles/ProfileManager.cpp
#include "ProfileManager.h"
#include "Core/Logger.hpp"
#include <SDL3/SDL.h>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <ctime>
#include <cstring>

namespace ShapeEngine::UI {

    static void GetWindowDims(SDL_Window* window, int& w, int& h) {
        w = 1280; h = 720;
        if (window) {
            SDL_GetWindowSize(window, &w, &h);
        }
    }

    ProfileManager::ProfileManager(const Config& config) 
        : m_config(config), m_window(config.window), m_renderer(config.renderer) {
        refreshProfiles();
    }

    ProfileManager::~ProfileManager() {
        for (auto& p : m_profiles) {
            if (p.preview) SDL_DestroyTexture(p.preview);
        }
    }

    void ProfileManager::refreshProfiles() {
        for (auto& p : m_profiles) {
            if (p.preview) SDL_DestroyTexture(p.preview);
        }
        m_profiles.clear();
        
        std::string profilesDir = "Content/_user/profiles";
        if (!std::filesystem::exists(profilesDir)) {
            std::filesystem::create_directories(profilesDir);
        }
        
        for (const auto& entry : std::filesystem::directory_iterator(profilesDir)) {
            if (entry.path().extension() != ".json") continue;
            
            try {
                Profile p;
                p.name = entry.path().stem().string();
                p.createdDate = getCurrentDateTime();
                p.lastUsedDate = p.createdDate;
                p.themeId = "deep-sea";
                p.settingsJson = "{}";
                p.isBuiltIn = false;
                p.preview = generatePreview(p);
                m_profiles.push_back(p);
            } catch (...) {
                SHAPE_LOG_WARN("Failed to load profile: %s", entry.path().string().c_str());
            }
        }
        
        if (m_profiles.empty()) {
            Profile defaultProf;
            defaultProf.name = "Default";
            defaultProf.createdDate = getCurrentDateTime();
            defaultProf.lastUsedDate = defaultProf.createdDate;
            defaultProf.themeId = "aurora";
            defaultProf.isBuiltIn = true;
            defaultProf.preview = generatePreview(defaultProf);
            m_profiles.push_back(defaultProf);
        }
        
        std::sort(m_profiles.begin(), m_profiles.end(), 
            [](const Profile& a, const Profile& b) {
                return a.lastUsedDate > b.lastUsedDate;
            });
    }

    bool ProfileManager::saveCurrentAsProfile(const std::string& name) {
        if (name.empty()) return false;
        
        Profile p;
        p.name = name;
        p.createdDate = getCurrentDateTime();
        p.lastUsedDate = p.createdDate;
        p.themeId = "deep-sea";
        p.settingsJson = "{}";
        p.preview = generatePreview(p);
        
        std::string path = "Content/_user/profiles/" + name + ".json";
        std::ofstream f(path);
        if (f.is_open()) {
            f << "{\n";
            f << "  \"name\": \"" << p.name << "\",\n";
            f << "  \"createdDate\": \"" << p.createdDate << "\",\n";
            f << "  \"lastUsedDate\": \"" << p.lastUsedDate << "\",\n";
            f << "  \"themeId\": \"" << p.themeId << "\"\n";
            f << "}\n";
        }
        
        m_profiles.push_back(p);
        SHAPE_LOG_INFO("Profile saved: %s", name.c_str());
        if (m_onSaved) m_onSaved(name);
        return true;
    }

    bool ProfileManager::loadProfile(const std::string& name) {
        auto it = std::find_if(m_profiles.begin(), m_profiles.end(),
            [&](const Profile& p) { return p.name == name; });
        if (it == m_profiles.end()) return false;
        
        m_activeProfile = name;
        it->lastUsedDate = getCurrentDateTime();
        
        SHAPE_LOG_INFO("Profile loaded: %s", name.c_str());
        if (m_onLoaded) m_onLoaded(name);
        return true;
    }

    bool ProfileManager::deleteProfile(const std::string& name) {
        auto it = std::find_if(m_profiles.begin(), m_profiles.end(),
            [&](const Profile& p) { return p.name == name; });
        if (it == m_profiles.end()) return false;
        if (it->isBuiltIn) return false;
        
        std::filesystem::remove("Content/_user/profiles/" + name + ".json");
        
        if (it->preview) SDL_DestroyTexture(it->preview);
        m_profiles.erase(it);
        
        SHAPE_LOG_INFO("Profile deleted: %s", name.c_str());
        return true;
    }

    void ProfileManager::showNameInputDialog() {
        m_showNameDialog = true;
        m_newNameBuffer[0] = '\0';
        m_dialogError.clear();
    }

    void ProfileManager::render() {
        if (!m_visible || !m_renderer) return;
        
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        int panelX = (winW - m_config.panelWidth) / 2;
        int panelY = (winH - m_config.panelHeight) / 2;
        
        SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 180);
        SDL_FRect fullScreen = {0.0f, 0.0f, (float)winW, (float)winH};
        SDL_RenderFillRect(m_renderer, &fullScreen);
        
        SDL_FRect panel = {(float)panelX, (float)panelY, (float)m_config.panelWidth, (float)m_config.panelHeight};
        SDL_SetRenderDrawColor(m_renderer, m_config.backgroundColor.r,
                                m_config.backgroundColor.g, 
                                m_config.backgroundColor.b, 255);
        SDL_RenderFillRect(m_renderer, &panel);
        SDL_SetRenderDrawColor(m_renderer, m_config.accentColor.r,
                                m_config.accentColor.g, 
                                m_config.accentColor.b, 200);
        SDL_RenderRect(m_renderer, &panel);
        
        renderText("Profiles", panelX + 20, panelY + 20, 24, m_config.textColor);
        renderText("Save/load complete setting configurations",
                   panelX + 20, panelY + 55, 12, m_config.secondaryTextColor);
        
        int closeSize = 30;
        SDL_FRect closeBtn = {(float)(panelX + m_config.panelWidth - closeSize - 10), 
                              (float)(panelY + 10), (float)closeSize, (float)closeSize};
        SDL_SetRenderDrawColor(m_renderer, 100, 60, 60, 255);
        SDL_RenderFillRect(m_renderer, &closeBtn);
        renderText("X", panelX + m_config.panelWidth - 25, panelY + 15, 20, 
                   {240, 240, 240, 255}, true);
        
        SDL_Rect newBtn = {panelX + 20, panelY + m_config.panelHeight - 50, 
                             150, 35};
        if (button(newBtn.x, newBtn.y, newBtn.w, newBtn.h, 
                    "+ New Profile", m_config.accentColor)) {
            showNameInputDialog();
        }
        
        renderProfileList();
        
        if (m_showNameDialog) renderNewProfileDialog();
        if (m_showDeleteConfirm) renderDeleteConfirmation();
    }

    void ProfileManager::renderProfileList() {
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        int panelX = (winW - m_config.panelWidth) / 2;
        int panelY = (winH - m_config.panelHeight) / 2;
        
        int listX = panelX + 20;
        int listY = panelY + 80;
        int listW = m_config.panelWidth - 40;
        int cardH = 80;
        int cardSpacing = 10;
        
        float mouseX = 0.0f, mouseY = 0.0f;
        SDL_GetMouseState(&mouseX, &mouseY);
        m_hoveredIndex = -1;
        
        for (size_t i = 0; i < m_profiles.size(); ++i) {
            int cardY = listY + (int)i * (cardH + cardSpacing) - m_scrollOffset;
            
            if (cardY + cardH < listY || cardY > listY + 360) continue;
            
            if (mouseX >= listX && mouseX <= listX + listW &&
                mouseY >= cardY && mouseY <= cardY + cardH) {
                m_hoveredIndex = (int)i;
            }
            
            renderProfileCard(m_profiles[i], (int)i);
        }
        
        if (m_profiles.empty()) {
            renderText("No profiles saved yet.", 
                       panelX + m_config.panelWidth / 2, 
                       panelY + 250, 14, m_config.secondaryTextColor, true);
            renderText("Click '+ New Profile' to save your current settings.",
                       panelX + m_config.panelWidth / 2, 
                       panelY + 275, 12, m_config.secondaryTextColor, true);
        }
    }

    void ProfileManager::renderProfileCard(Profile& profile, int index) {
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        int panelX = (winW - m_config.panelWidth) / 2;
        int panelY = (winH - m_config.panelHeight) / 2;
        
        int cardX = panelX + 20;
        int cardY = panelY + 80 + index * (80 + 10) - m_scrollOffset;
        int cardW = m_config.panelWidth - 40;
        int cardH = 80;
        
        bool hovered = (m_hoveredIndex == index);
        bool isActive = (profile.name == m_activeProfile);
        
        SDL_FRect cardRect = {(float)cardX, (float)cardY, (float)cardW, (float)cardH};
        
        ProfileColorRGBA bgColor = isActive ? m_config.successColor : 
                            hovered ? m_config.cardHoverColor : m_config.cardColor;
        bgColor.a = 220;
        SDL_SetRenderDrawColor(m_renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
        SDL_RenderFillRect(m_renderer, &cardRect);
        
        ProfileColorRGBA borderColor = isActive ? m_config.successColor : 
                               hovered ? m_config.accentColor : 
                                         ProfileColorRGBA{60, 70, 85, 255};
        SDL_SetRenderDrawColor(m_renderer, borderColor.r, borderColor.g, 
                                borderColor.b, 200);
        SDL_RenderRect(m_renderer, &cardRect);
        
        if (profile.preview) {
            SDL_FRect thumbRect = {(float)(cardX + 10), (float)(cardY + 10), 60.0f, 60.0f};
            SDL_RenderTexture(m_renderer, profile.preview, nullptr, &thumbRect);
        } else {
            SDL_FRect thumbRect = {(float)(cardX + 10), (float)(cardY + 10), 60.0f, 60.0f};
            SDL_SetRenderDrawColor(m_renderer, 50, 60, 75, 255);
            SDL_RenderFillRect(m_renderer, &thumbRect);
        }
        
        renderText(profile.name, cardX + 85, cardY + 15, 16, m_config.textColor);
        
        if (isActive) {
            renderText("ACTIVE", cardX + cardW - 70, cardY + 15, 11, 
                       m_config.successColor);
        }
        
        std::string lastUsed = "Last used: " + 
                              (profile.lastUsedDate.empty() ? 
                               std::string("Never") : profile.lastUsedDate);
        renderText(lastUsed, cardX + 85, cardY + 38, 11, m_config.secondaryTextColor);
        
        std::string themeInfo = "Theme: " + profile.themeId;
        renderText(themeInfo, cardX + 85, cardY + 54, 11, m_config.secondaryTextColor);
        
        int btnW = 60, btnH = 25;
        int btnY = cardY + cardH - btnH - 8;
        
        SDL_Rect loadBtn = {cardX + cardW - 200, btnY, btnW, btnH};
        if (button(loadBtn.x, loadBtn.y, loadBtn.w, loadBtn.h, 
                    "Load", m_config.accentColor)) {
            loadProfile(profile.name);
        }
        
        if (!profile.isBuiltIn) {
            SDL_Rect delBtn = {cardX + cardW - 130, btnY, btnW, btnH};
            if (button(delBtn.x, delBtn.y, delBtn.w, delBtn.h, 
                        "Delete", m_config.dangerColor)) {
                m_showDeleteConfirm = true;
                m_profileToDelete = profile.name;
            }
        }
    }

    void ProfileManager::renderNewProfileDialog() {
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        
        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 150);
        SDL_FRect fullScreen = {0.0f, 0.0f, (float)winW, (float)winH};
        SDL_RenderFillRect(m_renderer, &fullScreen);
        
        int dialogW = 400, dialogH = 180;
        int dialogX = (winW - dialogW) / 2;
        int dialogY = (winH - dialogH) / 2;
        SDL_FRect dialog = {(float)dialogX, (float)dialogY, (float)dialogW, (float)dialogH};
        SDL_SetRenderDrawColor(m_renderer, 30, 35, 45, 255);
        SDL_RenderFillRect(m_renderer, &dialog);
        SDL_SetRenderDrawColor(m_renderer, m_config.accentColor.r,
                                m_config.accentColor.g, m_config.accentColor.b, 255);
        SDL_RenderRect(m_renderer, &dialog);
        
        renderText("Save New Profile", dialogX + 20, dialogY + 15, 18, 
                   m_config.textColor);
        renderText("Enter a name for this profile:",
                   dialogX + 20, dialogY + 45, 12, m_config.secondaryTextColor);
        
        SDL_FRect inputBox = {(float)(dialogX + 20), (float)(dialogY + 70), (float)(dialogW - 40), 30.0f};
        SDL_SetRenderDrawColor(m_renderer, 20, 22, 28, 255);
        SDL_RenderFillRect(m_renderer, &inputBox);
        SDL_SetRenderDrawColor(m_renderer, 100, 180, 255, 255);
        SDL_RenderRect(m_renderer, &inputBox);
        
        if (m_newNameBuffer[0] != '\0') {
            renderText(m_newNameBuffer, dialogX + 30, dialogY + 77, 14, 
                       m_config.textColor);
        } else {
            renderText("Type a name...", dialogX + 30, dialogY + 77, 14, 
                       m_config.secondaryTextColor);
        }
        
        if (!m_dialogError.empty()) {
            renderText(m_dialogError, dialogX + 20, dialogY + 110, 11, 
                       m_config.dangerColor);
        }
        
        SDL_Rect saveBtn = {dialogX + 220, dialogY + 130, 80, 35};
        SDL_Rect cancelBtn = {dialogX + 305, dialogY + 130, 80, 35};
        
        if (button(saveBtn.x, saveBtn.y, saveBtn.w, saveBtn.h, 
                    "Save", m_config.successColor)) {
            if (strlen(m_newNameBuffer) == 0) {
                m_dialogError = "Please enter a name";
            } else {
                if (saveCurrentAsProfile(m_newNameBuffer)) {
                    m_showNameDialog = false;
                } else {
                    m_dialogError = "Profile name already exists";
                }
            }
        }
        
        if (button(cancelBtn.x, cancelBtn.y, cancelBtn.w, cancelBtn.h, 
                    "Cancel", m_config.dangerColor)) {
            m_showNameDialog = false;
        }
    }

    void ProfileManager::renderDeleteConfirmation() {
        int winW = 1280, winH = 720;
        GetWindowDims(m_window, winW, winH);
        
        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 150);
        SDL_FRect fullScreen = {0.0f, 0.0f, (float)winW, (float)winH};
        SDL_RenderFillRect(m_renderer, &fullScreen);
        
        int dialogW = 350, dialogH = 130;
        int dialogX = (winW - dialogW) / 2;
        int dialogY = (winH - dialogH) / 2;
        SDL_FRect dialog = {(float)dialogX, (float)dialogY, (float)dialogW, (float)dialogH};
        SDL_SetRenderDrawColor(m_renderer, 30, 35, 45, 255);
        SDL_RenderFillRect(m_renderer, &dialog);
        
        char buf[128];
        snprintf(buf, sizeof(buf), "Delete profile '%s'?", m_profileToDelete.c_str());
        renderText(buf, dialogX + 20, dialogY + 25, 16, m_config.textColor);
        renderText("This cannot be undone.", 
                   dialogX + 20, dialogY + 50, 12, m_config.secondaryTextColor);
        
        SDL_Rect yesBtn = {dialogX + 170, dialogY + 80, 80, 35};
        SDL_Rect noBtn = {dialogX + 255, dialogY + 80, 80, 35};
        
        if (button(yesBtn.x, yesBtn.y, yesBtn.w, yesBtn.h, 
                    "Delete", m_config.dangerColor)) {
            deleteProfile(m_profileToDelete);
            m_showDeleteConfirm = false;
        }
        if (button(noBtn.x, noBtn.y, noBtn.w, noBtn.h, 
                    "Cancel", {100, 100, 110, 255})) {
            m_showDeleteConfirm = false;
        }
    }

    SDL_Texture* ProfileManager::generatePreview(const Profile& profile) {
        if (!m_renderer) return nullptr;
        (void)profile;
        int size = 60;
        SDL_Surface* surface = SDL_CreateSurface(size, size, SDL_PIXELFORMAT_RGBA8888);
        if (!surface) return nullptr;
        
        SDL_LockSurface(surface);
        uint32_t* pixels = (uint32_t*)surface->pixels;
        const SDL_PixelFormatDetails* details = SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA8888);
        
        for (int y = 0; y < size; ++y) {
            for (int x = 0; x < size; ++x) {
                float t = (float)y / size;
                uint8_t r = (Uint8)(20 + t * 30);
                uint8_t g = (Uint8)(30 + t * 40);
                uint8_t b = (Uint8)(50 + t * 60);
                pixels[y * size + x] = SDL_MapRGBA(details, nullptr, r, g, b, 255);
            }
        }
        
        SDL_UnlockSurface(surface);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);
        SDL_DestroySurface(surface);
        return texture;
    }

    bool ProfileManager::handleInput() {
        if (!m_visible) return false;
        
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_KEY_DOWN) {
                if (event.key.key == SDLK_ESCAPE) {
                    if (m_showNameDialog) m_showNameDialog = false;
                    else if (m_showDeleteConfirm) m_showDeleteConfirm = false;
                    else hide();
                    return true;
                }
            }
            else if (event.type == SDL_EVENT_MOUSE_WHEEL) {
                m_scrollOffset -= (int)(event.wheel.y * 20.0f);
                m_scrollOffset = std::max(0, m_scrollOffset);
            }
        }
        return true;
    }

    void ProfileManager::update(float deltaTime) {
        (void)deltaTime;
    }

    bool ProfileManager::button(int x, int y, int w, int h, const std::string& label, 
                                  ProfileColorRGBA color) {
        if (!m_renderer) return false;
        float mouseX = 0.0f, mouseY = 0.0f;
        SDL_GetMouseState(&mouseX, &mouseY);
        bool hovered = (mouseX >= x && mouseX <= x + w && mouseY >= y && mouseY <= y + h);
        
        ProfileColorRGBA btnColor = hovered ? color : ProfileColorRGBA{
            (Uint8)(color.r * 0.7f), (Uint8)(color.g * 0.7f), 
            (Uint8)(color.b * 0.7f), color.a};
        SDL_SetRenderDrawColor(m_renderer, btnColor.r, btnColor.g, btnColor.b, 255);
        SDL_FRect btn = {(float)x, (float)y, (float)w, (float)h};
        SDL_RenderFillRect(m_renderer, &btn);
        
        renderText(label, x + w / 2, y + (h / 2 - 7), 13, 
                   {240, 240, 245, 255}, true);
        
        return false;
    }

    void ProfileManager::renderText(const std::string& text, int x, int y, int size, 
                                       ProfileColorRGBA color, bool centered) {
        if (!m_renderer) return;
        (void)size;
        SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
        float drawX = centered ? (float)x - (text.length() * 4.0f) : (float)x;
        SDL_RenderDebugText(m_renderer, drawX, (float)y, text.c_str());
    }

    std::string ProfileManager::getCurrentDateTime() {
        auto now = std::time(nullptr);
        char buf[32];
        std::tm tm_buf;
        #ifdef _WIN32
        localtime_s(&tm_buf, &now);
        #else
        localtime_r(&now, &tm_buf);
        #endif
        std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", &tm_buf);
        return buf;
    }

} // namespace ShapeEngine::UI
