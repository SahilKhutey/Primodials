// Shape/UI/IOverlayUI.h
// Immediate-mode overlay UI interface
#pragma once

#include "Shape/Math/Vec2.hpp"
#include "Shape/Math/Color.hpp"
#include <string>
#include <vector>
#include <functional>
#include <cstdint>
#include <memory>

namespace Shape::UI {

    enum class WidgetType : uint8_t {
        Panel,
        Label,
        Button,
        Checkbox,
        Slider,
        ProgressBar,
        Dropdown,
        TextInput,
        Image,
        Graph,
        Separator,
        Spacer
    };

    enum class MouseButton : uint8_t { Left, Middle, Right };

    struct UIState {
        Math::Vec2f mouse_pos{0, 0};
        Math::Vec2f mouse_delta{0, 0};
        bool mouse_down[3] = {};
        bool mouse_pressed[3] = {};
        bool mouse_released[3] = {};
        float mouse_wheel = 0;
        bool shift = false, ctrl = false, alt = false;
        bool typing = false;
        double time = 0;
        Math::Vec2f display_size{1280, 720};
    };

    class IOverlayUI {
    public:
        virtual ~IOverlayUI() = default;
        virtual void begin_frame(const UIState& state) = 0;
        virtual void end_frame() = 0;
        virtual void render() = 0;

        // Layout
        virtual void begin_panel(const char* id, const Math::Vec2f& pos, const Math::Vec2f& size) = 0;
        virtual void end_panel() = 0;
        virtual void separator() = 0;
        virtual void spacing() = 0;

        // Widgets
        virtual void label(const char* text) = 0;
        virtual void label_colored(const char* text, const Math::Color& c) = 0;
        virtual bool button(const char* label, const Math::Vec2f& size = {0, 0}) = 0;
        virtual bool checkbox(const char* label, bool& value) = 0;
        virtual bool slider_float(const char* label, float& value, float min, float max) = 0;
        virtual bool slider_int(const char* label, int& value, int min, int max) = 0;
        virtual void progress_bar(float fraction, const Math::Vec2f& size = {-1, 0}) = 0;
        virtual bool text_input(const char* label, std::string& buffer) = 0;
        virtual int  dropdown(const char* label, int current, const std::vector<std::string>& options) = 0;

        // Custom
        virtual void graph(const std::vector<float>& values, const Math::Vec2f& size = {200, 50}, const Math::Color& color = {0, 1, 0, 1}) = 0;
        virtual void line_chart(const std::vector<Math::Vec2f>& points, const Math::Vec2f& size = {200, 100}) = 0;
        virtual void colored_bar(const char* label, float value, float max, const Math::Color& color) = 0;
    };

    // Factory: creates an ImGui-backed overlay
    std::unique_ptr<IOverlayUI> create_imgui_overlay();

} // namespace Shape::UI
