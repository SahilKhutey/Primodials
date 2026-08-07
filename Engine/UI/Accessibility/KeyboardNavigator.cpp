// Engine/UI/Accessibility/KeyboardNavigator.cpp
#include "KeyboardNavigator.h"
#include "Core/Logger.hpp"
#include <SDL3/SDL.h>
#include <algorithm>
#include <cmath>

namespace ShapeEngine::UI {

    void KeyboardNavigator::registerElement(const FocusableElement& element) {
        unregisterElement(element.id);
        m_elements.push_back(element);
    }

    void KeyboardNavigator::unregisterElement(int id) {
        m_elements.erase(
            std::remove_if(m_elements.begin(), m_elements.end(),
                [id](const FocusableElement& e) { return e.id == id; }),
            m_elements.end());
        
        if (m_focusedId == id) {
            m_focusedId = -1;
        }
    }

    void KeyboardNavigator::clearElements() {
        m_elements.clear();
        m_focusedId = -1;
    }

    void KeyboardNavigator::focusNext() {
        if (m_elements.empty()) return;
        
        std::vector<int> order;
        if (!m_tabOrder.empty()) {
            order = m_tabOrder;
        } else {
            for (const auto& e : m_elements) order.push_back(e.id);
        }
        
        auto it = std::find(order.begin(), order.end(), m_focusedId);
        
        for (size_t i = 0; i < order.size(); ++i) {
            size_t idx = (it == order.end()) ? 0 : 
                         ((it - order.begin() + i + 1) % order.size());
            int id = order[idx];
            
            auto elem = std::find_if(m_elements.begin(), m_elements.end(),
                [id](const FocusableElement& e) { return e.id == id; });
            
            if (elem != m_elements.end() && elem->enabled && elem->visible) {
                focusElement(id);
                return;
            }
        }
    }

    void KeyboardNavigator::focusPrevious() {
        if (m_elements.empty()) return;
        
        std::vector<int> order;
        if (!m_tabOrder.empty()) {
            order = m_tabOrder;
        } else {
            for (const auto& e : m_elements) order.push_back(e.id);
        }
        
        auto it = std::find(order.begin(), order.end(), m_focusedId);
        
        for (size_t i = 0; i < order.size(); ++i) {
            size_t idx = (it == order.begin() || it == order.end()) ? 
                         (order.size() - 1) :
                         ((it - order.begin() - i - 1 + order.size()) % order.size());
            int id = order[idx];
            
            auto elem = std::find_if(m_elements.begin(), m_elements.end(),
                [id](const FocusableElement& e) { return e.id == id; });
            
            if (elem != m_elements.end() && elem->enabled && elem->visible) {
                focusElement(id);
                return;
            }
        }
    }

    void KeyboardNavigator::focusElement(int id) {
        auto it = std::find_if(m_elements.begin(), m_elements.end(),
            [id](const FocusableElement& e) { return e.id == id; });
        
        if (it == m_elements.end() || !it->enabled || !it->visible) return;
        
        m_focusedId = id;
        if (it->onFocus) it->onFocus();
        SHAPE_LOG_INFO("Focus: %s", it->name.c_str());
    }

    void KeyboardNavigator::clearFocus() {
        if (m_focusedId != -1) {
            SHAPE_LOG_INFO("Focus cleared");
        }
        m_focusedId = -1;
    }

    void KeyboardNavigator::setTabOrder(const std::vector<int>& order) {
        m_tabOrder = order;
    }

    bool KeyboardNavigator::handleKey(SDL_Keycode key) {
        if (!m_enabled) return false;
        
        switch (key) {
            case SDLK_TAB:
                if (SDL_GetModState() & SDL_KMOD_SHIFT) {
                    focusPrevious();
                } else {
                    focusNext();
                }
                return true;
                
            case SDLK_RETURN:
            case SDLK_SPACE: {
                auto it = std::find_if(m_elements.begin(), m_elements.end(),
                    [id = m_focusedId](const FocusableElement& e) { return e.id == id; });
                if (it != m_elements.end() && it->onActivate) {
                    it->onActivate();
                    return true;
                }
                return false;
            }
                
            case SDLK_ESCAPE:
                clearFocus();
                return true;
                
            default:
                return false;
        }
    }

    void KeyboardNavigator::renderFocusIndicator(SDL_Renderer* renderer) const {
        if (!renderer || m_focusedId == -1) return;
        
        auto it = std::find_if(m_elements.begin(), m_elements.end(),
            [id = m_focusedId](const FocusableElement& e) { return e.id == id; });
        if (it == m_elements.end()) return;
        
        uint64_t ticks = SDL_GetTicks();
        float pulse = (sinf((float)ticks / 300.0f) + 1.0f) * 0.5f;
        
        SDL_Rect bounds = it->bounds;
        SDL_SetRenderDrawColor(renderer, 100, 180, 255, 
                                (Uint8)(150 + pulse * 105));
        
        for (int i = 0; i < 3; ++i) {
            SDL_FRect border = {
                (float)(bounds.x - i * 2), (float)(bounds.y - i * 2),
                (float)(bounds.w + i * 4), (float)(bounds.h + i * 4)
            };
            SDL_RenderRect(renderer, &border);
        }
    }

} // namespace ShapeEngine::UI
