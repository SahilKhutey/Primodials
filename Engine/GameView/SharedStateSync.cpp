// Engine/GameView/SharedStateSync.cpp
#include "SharedStateSync.h"
#include "Core/Logger.hpp"

#ifdef _WIN32
    #include <windows.h>
#elif defined(__linux__) || defined(__APPLE__)
    #include <sys/mman.h>
    #include <sys/stat.h>
    #include <fcntl.h>
    #include <unistd.h>
#endif

#include <cstring>
#include <chrono>

namespace ShapeEngine::GameView {

    SharedStateSync::SharedStateSync() = default;
    SharedStateSync::~SharedStateSync() { shutdown(); }

    void SharedStateSync::initialize(Shape::World* world, SyncMode mode) {
        m_world = world;
        m_mode = mode;
        
        if (mode == SyncMode::MultiProcess) {
            initializeMultiProcess();
        }
        
        m_lastSyncTimestamp = (uint64_t)std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
        
        SHAPE_LOG_INFO("SharedStateSync initialized (mode: %s)", 
                 mode == SyncMode::SingleProcess ? "single-process" : "multi-process");
    }

    void SharedStateSync::shutdown() {
        if (m_mode == SyncMode::MultiProcess && m_sharedMemory) {
#ifdef _WIN32
            UnmapViewOfFile(m_sharedMemory);
#elif defined(__linux__) || defined(__APPLE__)
            munmap(m_sharedMemory, m_sharedMemorySize);
            shm_unlink(m_sharedMemoryName.c_str());
#endif
            m_sharedMemory = nullptr;
        }
    }

    void SharedStateSync::initializeMultiProcess() {
#ifdef _WIN32
        m_sharedMemorySize = 16 * 1024 * 1024;
        m_sharedMemoryName = "Local\\PolygonalPrimordials_SharedState";
        
        HANDLE hMapFile = CreateFileMappingA(
            INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE,
            0, (DWORD)m_sharedMemorySize, m_sharedMemoryName.c_str());
        
        if (hMapFile == nullptr) {
            SHAPE_LOG_ERROR("Could not create file mapping: %lu", GetLastError());
            return;
        }
        
        m_sharedMemory = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 
                                         m_sharedMemorySize);
        CloseHandle(hMapFile);
        
#elif defined(__linux__) || defined(__APPLE__)
        m_sharedMemorySize = 16 * 1024 * 1024;
        m_sharedMemoryName = "/polygonal_primordials_state";
        
        int fd = shm_open(m_sharedMemoryName.c_str(), 
                           O_CREAT | O_RDWR, 0666);
        if (fd == -1) {
            SHAPE_LOG_ERROR("Could not create shared memory");
            return;
        }
        
        if (ftruncate(fd, m_sharedMemorySize) == -1) {
            SHAPE_LOG_ERROR("Could not resize shared memory segment");
            close(fd);
            return;
        }
        m_sharedMemory = mmap(nullptr, m_sharedMemorySize, 
                                PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        close(fd);
#endif
        SHAPE_LOG_INFO("Multi-process shared memory initialized: %s", m_sharedMemoryName.c_str());
    }

    SharedStateSync::Snapshot SharedStateSync::takeLightSnapshot() {
        std::lock_guard<std::mutex> lock(m_mutex);
        Snapshot s;
        
        if (!m_world) return s;
        
        s.tickNumber = m_world->GetTickCount();
        s.entityCount = (uint32_t)m_world->GetActiveEntityCount();
        
        s.timestamp = (uint64_t)std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
        
        m_hasUpdates = false;
        return s;
    }

    SharedStateSync::Snapshot SharedStateSync::takeFullSnapshot() {
        std::lock_guard<std::mutex> lock(m_mutex);
        Snapshot s = takeLightSnapshot();
        return s;
    }

    void SharedStateSync::applySnapshot(const Snapshot& snapshot) {
        std::lock_guard<std::mutex> lock(m_mutex);
        (void)snapshot;
        if (!m_world) return;
        
        m_hasUpdates = true;
        m_lastSyncTimestamp = (uint64_t)std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
    }

    bool SharedStateSync::hasRemoteUpdates() const {
        return m_hasUpdates.load();
    }

    void SharedStateSync::queueCommand(const Command& cmd) {
        std::lock_guard<std::mutex> lock(m_commandMutex);
        m_commandQueue.push_back(cmd);
        
        if (m_mode == SyncMode::MultiProcess) {
            sendCommandMultiProcess(cmd);
        }
    }

    std::vector<SharedStateSync::Command> SharedStateSync::getPendingCommands() {
        std::lock_guard<std::mutex> lock(m_commandMutex);
        return m_commandQueue;
    }

    void SharedStateSync::clearCommands() {
        std::lock_guard<std::mutex> lock(m_commandMutex);
        m_commandQueue.clear();
    }

    void SharedStateSync::sendCommandMultiProcess(const Command& cmd) {
        if (!m_sharedMemory) return;
        
        char* mem = (char*)m_sharedMemory + 4096;
        
        int offset = 0;
        int cmdType = (int)cmd.type;
        std::memcpy(mem + offset, &cmdType, sizeof(int));
        offset += sizeof(int);
        
        size_t paramLen = cmd.param1.size();
        std::memcpy(mem + offset, &paramLen, sizeof(size_t));
        offset += sizeof(size_t);
        if (paramLen > 0) std::memcpy(mem + offset, cmd.param1.data(), paramLen);
        offset += (int)paramLen;
        
        paramLen = cmd.param2.size();
        std::memcpy(mem + offset, &paramLen, sizeof(size_t));
        offset += sizeof(size_t);
        if (paramLen > 0) std::memcpy(mem + offset, cmd.param2.data(), paramLen);
        offset += (int)paramLen;
        
        std::memcpy(mem + offset, &cmd.floatParam, sizeof(float));
        offset += sizeof(float);
        std::memcpy(mem + offset, &cmd.intParam, sizeof(int));
    }

    void SharedStateSync::pollMultiProcessUpdates() {
        if (!m_sharedMemory) return;
        
        char* mem = (char*)m_sharedMemory;
        uint64_t timestamp;
        std::memcpy(&timestamp, mem, sizeof(uint64_t));
        
        if (timestamp > m_lastSyncTimestamp) {
            m_hasUpdates = true;
        }
    }

} // namespace ShapeEngine::GameView
