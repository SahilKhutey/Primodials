#include "Jobs/JobSystem.hpp"
#include "Core/Logger.hpp"

namespace shape {

JobSystem& JobSystem::Get() {
    static JobSystem instance;
    return instance;
}

JobSystem::~JobSystem() {
    Shutdown();
}

void JobSystem::Initialize() {
    if (!m_workers.empty()) {
        return;
    }

    u32 numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0) {
        numThreads = 2; // Fallback
    }

    SHAPE_LOG_INFO("JobSystem: Starting thread pool with worker count: {}", numThreads);
    m_stop = false;
    m_activeTasksCount = 0;

    for (u32 i = 0; i < numThreads; ++i) {
        m_workers.emplace_back(&JobSystem::WorkerLoop, this, i);
    }
}

void JobSystem::Schedule(JobTask task) {
    {
        std::lock_guard<std::mutex> lock(m_queueMutex);
        m_tasks.push(task);
        m_activeTasksCount++;
    }
    m_cv.notify_one();
}


void JobSystem::ParallelFor(u32 count, u32 batchSize, std::function<void(u32 start, u32 end)> func) {
    if (count == 0) return;
    if (batchSize == 0 || m_workers.empty()) {
        func(0, count);
        return;
    }

    u32 numBatches = (count + batchSize - 1) / batchSize;
    for (u32 i = 0; i < numBatches; ++i) {
        u32 start = i * batchSize;
        u32 end = std::min(start + batchSize, count);
        Schedule([start, end, func]() {
            func(start, end);
        });
    }

    WaitAll();
}

void JobSystem::WaitAll() {
    while (m_activeTasksCount > 0) {
        std::this_thread::yield();
    }
}

void JobSystem::Shutdown() {
    if (m_workers.empty()) {
        return;
    }

    {
        std::lock_guard<std::mutex> lock(m_queueMutex);
        m_stop = true;
    }
    m_cv.notify_all();

    for (auto& worker : m_workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
    m_workers.clear();
    SHAPE_LOG_INFO("JobSystem: All worker threads joined and shutdown.");
}

void JobSystem::WorkerLoop(u32 threadIndex) {
    (void)threadIndex;

    while (true) {
        JobTask task;
        {
            std::unique_lock<std::mutex> lock(m_queueMutex);
            m_cv.wait(lock, [this]() { return m_stop || !m_tasks.empty(); });

            if (m_stop && m_tasks.empty()) {
                return;
            }

            task = std::move(m_tasks.front());
            m_tasks.pop();
        }

        if (task) {
            task();
            m_activeTasksCount--;
        }
    }
}

} // namespace shape
