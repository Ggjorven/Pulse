#include "plpch.h"
#include "ThreadPool.hpp"

namespace Pulse
{

    ThreadPool::ThreadPool(size_t numThreads)
    {
        for (size_t i = 0; i < numThreads; ++i)
            m_Workers.emplace_back([this] { this->Worker(); });
    }

    ThreadPool::~ThreadPool() 
    {
        m_Stop = true;
        m_Condition.notify_all();

        for (std::thread& worker : m_Workers)
        {
            if (worker.joinable())
                worker.join();
        }
    }

    void ThreadPool::Enqueue(std::function<void()> task)
    {
        {
            std::unique_lock<std::mutex> lock(m_QueueMutex);
            m_Tasks.emplace(std::move(task));
        }

        m_Condition.notify_one();
    }

    void ThreadPool::Worker() 
    {
        while (!m_Stop) 
        {
            std::function<void()> task;
            
            {
                std::unique_lock<std::mutex> lock(m_QueueMutex);

                m_Condition.wait(lock, [this] { return !m_Tasks.empty() || m_Stop; });

                // Prematurely stop
                if (m_Stop && m_Tasks.empty()) return;

                task = std::move(m_Tasks.front());
                m_Tasks.pop();
            }

            task();
        }
    }

}