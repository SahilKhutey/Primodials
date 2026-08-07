// Engine/UI/Accessibility/KeyboardNavigator.h
#pragma once

#include <SDL3/SDL.h>
#include <vector>
#include <functional>
#include <string>

namespace ShapeEngine::UI {

    /// <summary>
    /// Provides Tab/Shift+Tab navigation between focusable UI elements.
    /// Required for accessibility (keyboard-only operation).
    /// </summary>
    class KeyboardNavigator {
    public:
        struct FocusableElement {
            int id = 0;
            std::string name;
            SDL_Rect bounds{0, 0, 0, 0};
            bool enabled = true;
            bool visible = true;
            std::function<void()> onActivate = nullptr;  // For Enter/Space
            std::function<void()> onFocus = nullptr;     // Called when focused
        };

        void registerElement(const FocusableElement& element);
        void unregisterElement(int id);
        void clearElements();

        void focusNext();
        void focusPrevious();
        void focusElement(int id);
        void clearFocus();

        int getFocusedId() const { return m_focusedId; }
        bool hasFocus() const { return m_focusedId != -1; }

        void setEnabled(bool enabled) { m_enabled = enabled; }
        bool isEnabled() const { return m_enabled; }

        void setTabOrder(const std::vector<int>& order);
        bool handleKey(SDL_Keycode key);
        void renderFocusIndicator(SDL_Renderer* renderer) const;

    private:
        std::vector<FocusableElement> m_elements;
        int m_focusedId = -1;
        bool m_enabled = true;
        std::vector<int> m_tabOrder;
    };

} // namespace ShapeEngine::UI
