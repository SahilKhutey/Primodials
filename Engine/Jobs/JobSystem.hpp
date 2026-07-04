#pragma once

#include "Core/Platform.hpp"
#include <functional>
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>

namespace shape {

using JobTask = std::function<void()>;

class JobSystem {
public:
    static JobSystem& Get();

    void Initialize();
    void Schedule(JobTask task);
    void WaitAll();
    void ParallelFor(u32 count, u32 batchSize, std::function<void(u32 start, u32 end)> func);
    void Shutdown();

private:
    JobSystem() = default;
    ~JobSystem();

    void WorkerLoop(u32 threadIndex);

    std::vector<std::thread> m_workers;
    std::queue<JobTask> m_tasks;
    std::mutex m_queueMutex;
    std::condition_variable m_cv;
    std::atomic<bool> m_stop{false};
    std::atomic<u32> m_activeTasksCount{0};
};

} // namespace shape
