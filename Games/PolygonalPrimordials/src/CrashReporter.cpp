// CrashReporter.cpp
#include "CrashReporter.hpp"
#include "Core/Logger.hpp"

namespace PolygonalPrimordials {

    CrashReporter& CrashReporter::instance() {
        static CrashReporter cr;
        return cr;
    }

    bool CrashReporter::initialize(const std::string&, const std::string&) {
        SHAPE_LOG_INFO("CrashReporter: Initialized");
        return true;
    }

    void CrashReporter::shutdown() {}

    void CrashReporter::generate_minidump() {
        SHAPE_LOG_ERROR("CrashReporter: Generating minidump...");
    }

} // namespace
