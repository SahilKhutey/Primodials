// Localization.h
#pragma once
#include "Core/NonCopyable.hpp"
#include <string>

namespace PolygonalPrimordials {

    class Localization : public Shape::NonCopyable {
    public:
        static Localization& instance() { static Localization l; return l; }
        void load(const std::string&) {}
        void set_active(const std::string&) {}
        std::string get(const std::string& key) const { return key; }
    };

}
