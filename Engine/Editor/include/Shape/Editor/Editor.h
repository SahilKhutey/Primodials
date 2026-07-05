#pragma once
#include <memory>

namespace Shape::Editor {
    class Editor {
    public:
        Editor();
        ~Editor();
        
        bool initialize();
        void shutdown();
        void update(double dt);
        void render();
        
    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };
}
