#include "Shape/Editor/Editor.h"

namespace Shape::Editor {
    struct Editor::Impl {};
    Editor::Editor() : m_impl(std::make_unique<Impl>()) {}
    Editor::~Editor() = default;
    
    bool Editor::initialize() { return true; }
    void Editor::shutdown() {}
    void Editor::update(double dt) {}
    void Editor::render() {}
}
