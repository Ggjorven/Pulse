#pragma once

#include "Pulse/Thread/Mutex.hpp"
#include "Pulse/Thread/Thread.hpp"

#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>

namespace Pulse
{

    // Note: All threading functionality is currently stripped from macos
    // due to compilation errors with xcode.
#if defined(PULSE_PLATFORM_WINDOWS) || defined(PULSE_PLATFORM_LINUX)
    class ThreadPool
    {
    public:
        ThreadPool(size_t numThreads = (std::thread::hardware_concurrency() / 2u - 1));
        ~ThreadPool();

        void Enqueue(std::function<void()> task);

    private:
        void Worker();

    private:
        std::vector<Thread> m_Workers;
        std::queue<std::function<void()>> m_Tasks;
        Mutex m_QueueMutex;
        std::condition_variable m_Condition;
        std::atomic<bool> m_Stop = false;
    };
#endif

}