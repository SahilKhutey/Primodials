// Accessibility.h
#pragma once
#include "Shape/Core/NonCopyable.h"
#include <string>

namespace PolygonalPrimordials {

    class Accessibility : public Shape::NonCopyable {
    public:
        static Accessibility& instance() { static Accessibility a; return a; }
        void load_defaults(const std::string&) {}
    };

}
