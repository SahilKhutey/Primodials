#include <catch2/catch_all.hpp>
#include "ShapeEngine/Platform/Platform.hpp"
#include "ShapeEngine/Platform/FileSystem.hpp"
#include "ShapeEngine/Platform/Clock.hpp"
#include "ShapeEngine/Platform/Thread.hpp"
#include "ShapeEngine/Platform/Mutex.hpp"
#include "ShapeEngine/Platform/SystemInfo.hpp"
#include "ShapeEngine/Platform/Clipboard.hpp"
#include "ShapeEngine/Platform/Monitor.hpp"
#include "ShapeEngine/Platform/Process.hpp"
#include "ShapeEngine/Platform/Window.hpp"

namespace Shape {
class MockWindow final : public Window {
public:
    bool Create(const WindowDesc&) override { return true; }
    void Destroy() override {}
    void PollEvents() override {}
    bool ShouldClose() const override { return false; }
};
}

#include <atomic>

using namespace Shape;

TEST_CASE("Platform Layer API tests", "[Platform]") {
    REQUIRE(Platform::Initialize());

    SECTION("FileSystem operations") {
        std::filesystem::path testDir = "test_platform_dir";
        REQUIRE(FileSystem::CreateDirectory(testDir));
        REQUIRE(FileSystem::Exists(testDir));
        REQUIRE(FileSystem::Remove(testDir));
        REQUIRE_FALSE(FileSystem::Exists(testDir));
    }

    SECTION("Clock precision timers") {
        f64 startSec = Clock::Seconds();
        f64 startMs = Clock::Milliseconds();
        Process::Sleep(10);
        f64 endSec = Clock::Seconds();
        f64 endMs = Clock::Milliseconds();

        REQUIRE(endSec > startSec);
        REQUIRE(endMs > startMs);
    }

    SECTION("Thread and Mutex synchronizations") {
        Mutex mutex;
        std::atomic<u32> count{0};

        auto worker = [&]() {
            ScopedLock lock(mutex);
            count++;
        };

        Thread t1(worker);
        Thread t2(worker);
        t1.join();
        t2.join();

        REQUIRE(count == 2);
    }

    SECTION("SystemInfo queries") {
        CPUInfo info = SystemInfo::GetCPUInfo();
        REQUIRE(info.LogicalCores > 0);
        REQUIRE(info.RAM > 0);
        REQUIRE(info.AVX2);
        REQUIRE(info.SSE42);
    }

    SECTION("Clipboard operations") {
        std::string sample = "Hello Shape Engine Clipboard";
        Clipboard::SetText(sample);
        REQUIRE(Clipboard::GetText() == sample);
    }

    SECTION("Monitor dimensions") {
        MonitorInfo info = Monitor::Primary();
        REQUIRE(info.Width > 0);
        REQUIRE(info.Height > 0);
        REQUIRE(info.DPI > 0.0f);
    }

    SECTION("Process metadata") {
        REQUIRE(Process::ID() > 0);
    }

    SECTION("Window Creation lifecycle compiles") {
        MockWindow window;
        WindowDesc desc;
        desc.Title = "Test Window Compile";
        desc.Width = 320;
        desc.Height = 240;

        REQUIRE(window.Create(desc));
        window.PollEvents();
        REQUIRE_FALSE(window.ShouldClose());
        window.Destroy();
    }

    Platform::Shutdown();
}
