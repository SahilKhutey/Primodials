// CrashReporter.h
#pragma once
#include "Shape/Core/NonCopyable.h"
#include <string>

namespace PolygonalPrimordials {

    class CrashReporter : public Shape::NonCopyable {
    public:
        static CrashReporter& instance();

        bool initialize(const std::string& app_name, const std::string& version);
        void shutdown();

        void generate_minidump();

    private:
        CrashReporter() = default;
    };

} // namespace
